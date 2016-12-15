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
