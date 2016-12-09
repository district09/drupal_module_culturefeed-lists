<?php

/**
 * @file
 * Functionality to manage the lists an event belongs to.
 */

use \CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * Form to show the possible lists the event belongs to.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 * @param ActivityStatsExtendedEntity $event
 *   The event to show the form for.
 *
 * @return array
 *   The form structure.
 *
 * @see culturefeed_lists_manage_event_lists_form_submit()
 */
function culturefeed_lists_manage_event_lists_form(array $form, array &$form_state, ActivityStatsExtendedEntity $event) {
  $user_can_administer = user_access(CULTUREFEED_LISTS_PERMISSION_ADMINISTER);

  $event = $event->getEntity();
  $form_state['event'] = $event;

  // Add the lists table.
  $form['lists'] = culturefeed_lists_manage_event_lists_form_lists_table($event);

  // Add links to administer lists.
  if ($user_can_administer) {
    $links = array(
      l(
        t('Manage lists'),
        culturefeed_lists_admin_path()
      ),
      l(
        t('Create new list'),
        culturefeed_lists_admin_path(array('add')),
        array('query' => array('destination' => current_path()))
      ),
    );
    $form['lists_admin'] = array(
      '#markup' => implode(' | ', $links),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  return $form;
}

/**
 * Create the table form element array for the event lists form.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event to create the list for.
 *
 * @return array
 *   The form table structure.
 */
function culturefeed_lists_manage_event_lists_form_lists_table(\CultureFeed_Cdb_Item_Event $event) {
  $user_can_administer = user_access(CULTUREFEED_LISTS_PERMISSION_ADMINISTER);
  $event_lists = culturefeed_lists_event_load_lists($event);
  $lists = culturefeed_lists_list_load_all();

  $header = array('name' => t('List'));
  if ($user_can_administer) {
    $header['operations'] = t('Operations');
  }

  $lists_options = array();
  foreach ($lists as $list) {
    $list_id = $list->getId();
    $lists_options[$list_id] = array(
      'name' => $list->getName(),
    );

    // Manage list link.
    if ($user_can_administer) {
      $lists_options[$list_id]['operations'] = l(
        t('Manage list'),
        culturefeed_lists_admin_path(array($list_id, 'manage'))
      );
    }
  }

  $event_lists_ids = array_keys($event_lists->toArray());
  $default_values = array_fill_keys($event_lists_ids, TRUE);
  return array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $lists_options,
    '#default_value' => $default_values,
    '#empty' => t('No lists defined.'),
  );
}

/**
 * Submit handler for culturefeed_lists_manage_event_lists_form().
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 *
 * @see culturefeed_lists_manage_event_lists_form()
 */
function culturefeed_lists_manage_event_lists_form_submit(array $form, array &$form_state) {
  // Get the selected lists.
  $lists_all = culturefeed_lists_list_load_all()->toArray();
  $lists_selected = array_filter($form_state['values']['lists']);
  $lists = new CulturefeedListsListCollection(
    array_intersect_key($lists_all, $lists_selected)
  );

  // Update the event.
  try {
    culturefeed_lists_event_update_lists($form_state['event'], $lists);
    drupal_set_message(t('The lists are updated.'));
  }
  catch (Exception $e) {
    if ($e->getCode() === CultureFeed_EntryApi::CODE_KEYWORD_PRIVATE) {
      drupal_set_message(
        t('You tried to add the item to a list with a name that is not allowed.'),
        'error'
      );
    }
    else {
      drupal_set_message(
        t('Error occurred while saving the item lists.'),
        'error'
      );
    }
  }
}

/**
 * Form to replace the tags form on the agenda/e/EVENTID/tags page.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The form state.
 * @param ActivityStatsExtendedEntity $event
 *   The event to display the form for.
 *
 * @return array
 *   Form structure.
 *
 * @see culturefeed_entry_ui_edit_tag_objects_form()
 */
function culturefeed_lists_entry_ui_edit_tag_objects_form(array $form, array &$form_state, ActivityStatsExtendedEntity $event) {
  // Make sure that the original file is loaded.
  form_load_include(
    $form_state,
    'inc',
    'culturefeed_entry_ui',
    'includes/form/culturefeed_entry_ui.tag_objects'
  );

  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $event = $event->getEntity();
  /* @var \CultureFeed_Cdb_Item_Event $event */
  /* @var \CultureFeed_Cdb_Data_Keyword[] $event_tags */
  $custom_tags = variable_get('culturefeed_entry_ui_custom_tags', array());
  $lists = culturefeed_lists_list_load_all()->toArray();

  // Get the event tags without the lists.
  $event_tags = array_diff_key($event->getKeywords(TRUE), $lists);

  // Remove the lists from the old tags.
  $form['#old_tags'] = $event_tags;
  $form['#object_id'] = $event->getCdbId();

  // Tags.
  $tags = array();
  foreach ($event_tags as $tag) {
    $value = $tag->getValue();

    // Not a custom tag.
    if (array_key_exists($value, $custom_tags)) {
      continue;
    }

    $tags[] = array(
      'value' => $tag->getValue(),
      'visible' => $tag->isVisible(),
    );
  }

  $form['tags'] = array(
    '#default_value' => $tags,
    '#title' => t('Tags'),
    '#type' => 'culturefeed_tags_element',
  );

  if (user_access('manage custom tags') && !empty($custom_tags)) {
    $form['custom_tags'] = array(
      '#default_value' => array_intersect(array_keys($event_tags), array_keys($custom_tags)),
      '#options' => array_combine(array_keys($custom_tags), array_keys($custom_tags)),
      '#title' => t('Custom tags'),
      '#type' => 'checkboxes',
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#validate' => array('culturefeed_entry_ui_edit_tag_objects_form_submit_validate'),
      '#value' => t('Save'),
    ),
  );

  $form['#submit'][] = 'culturefeed_entry_ui_edit_tag_objects_form_submit';

  return $form;
}
