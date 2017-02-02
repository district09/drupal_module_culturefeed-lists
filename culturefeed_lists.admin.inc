<?php

/**
 * @file
 * Administrative pages and forms.
 */

use CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * Form to manage the event lists.
 */
function culturefeed_lists_admin_overview() {
  $output = array();

  $lists = culturefeed_lists_list_load_all();
  $output['lists'] = culturefeed_lists_admin_overview_table($lists);

  drupal_alter('culturefeed_lists_admin_overview', $output);

  return $output;
}

/**
 * Table overview of all events.
 *
 * @param CulturefeedListsListStore $lists
 *   The list items.
 *
 * @return array
 *   The table render array.
 */
function culturefeed_lists_admin_overview_table(CulturefeedListsListStore $lists) {
  $header = array(
    'name' => array('data' => t('Name')),
    'operations' => array('data' => t('Operations')),
  );

  $rows = array();
  foreach ($lists as $list) {
    $id = $list->getId();
    $name = $list->getName();

    $operations = array(
      'manage' => array(
        'title' => t('Manage items'),
        'href' => culturefeed_lists_admin_path(array($id, 'manage')),
      ),
      'edit' => array(
        'title' => t('Edit'),
        'href' => culturefeed_lists_admin_path(array($id, 'edit')),
        'query' => drupal_get_destination(),
      ),
      'delete' => array(
        'title' => t('Delete'),
        'href' => culturefeed_lists_admin_path(array($id, 'delete')),
        'query' => drupal_get_destination(),
      ),
    );
    $links = theme(
      'links',
      array(
        'links' => $operations,
        'attributes' => array('class' => array('links', 'inline')),
      )
    );

    $rows[] = array(
      'data' => array(
        'name' => array(
          'data' => $name,
        ),
        'operations' => array(
          'data' => $links,
        ),
      ),
      'list' => $list,
    );
  }

  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No lists defined.'),
  );
}

/**
 * Form to create a new or edit an existing list.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The form state.
 * @param CulturefeedListsList $list
 *   List definition (optional).
 *
 * @return array
 *   Form structure.
 *
 * @see culturefeed_lists_admin_list_form_submit()
 * @see culturefeed_lists_admin_list_form_submit_delete()
 * @see culturefeed_lists_admin_list_form_submit_cancel()
 */
function culturefeed_lists_admin_list_form(array $form, array &$form_state, CulturefeedListsList $list = NULL) {
  // Adding a new list is a local action. Local actions inherit the page title
  // by default from the parent item. We change the page title when we are on
  // the list add page. We check the path arguments to avoid overriding the
  // page title when the form is included on other pages.
  if (current_path() === culturefeed_lists_admin_path(array('add'))) {
    drupal_set_title(t('Add list'));
  }

  // Add the current list.
  $form['#list'] = $list;

  // The list name field.
  $default_name = $list
    ? $list->getName()
    : NULL;
  $form['list_name'] = array(
    '#type' => 'textfield',
    '#title' => t('List name'),
    '#default_value' => $default_name,
    '#required' => TRUE,
    '#element_validate' => array(
      'culturefeed_lists_admin_list_form_validate_list_name',
    ),
  );

  // The list path field.
  $default_path = $list
    ? $list->getPath()
    : NULL;
  $form['list_path'] = array(
    '#type' => 'machine_name',
    '#title' => t('List path'),
    '#default_value' => $default_path,
    '#maxlength' => MENU_MAX_MENU_NAME_LENGTH_UI,
    '#description' => t('A unique path to construct the URL for the list. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'culturefeed_lists_list_path_exists',
      'source' => array('list_name'),
      'label' => t('List path'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#element_validate' => array(
      'culturefeed_lists_admin_list_form_validate_list_path',
    ),
  );

  // Actions.
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($list) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#limit_validation_errors' => array(),
      '#submit' => array('culturefeed_lists_admin_list_form_submit_delete'),
    );
  }
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#limit_validation_errors' => array(),
    '#submit' => array('culturefeed_lists_admin_list_form_submit_cancel'),
  );

  return $form;
}

/**
 * Form element validator for the list_name field.
 *
 * @param array $element
 *   The element to validate.
 * @param array $form_state
 *   The current form state.
 * @param array $form
 *   The form structure.
 */
function culturefeed_lists_admin_list_form_validate_list_name(array $element, array &$form_state, array $form) {
  $list_name = $element['#value'];
  $list_edit = $form['#list'];

  // No error when the existing name did not change.
  if ($list_edit && $list_edit->getName() === $list_name) {
    return;
  }

  $error = FALSE;

  // Get the list with the same name (if any).
  $lists = culturefeed_lists_list_load_all();
  foreach ($lists as $list) {
    // Ignore self.
    if ($list_edit && $list_edit->getId() === $list->getId()) {
      continue;
    }

    if (drupal_strtolower($list->getName()) === drupal_strtolower($list_name)) {
      $error = t('There is already a list with the same name.');
      break;
    }
  }

  if ($error) {
    form_error($element, $error);
  }
}

/**
 * Form element validator for the list_path field.
 *
 * @param array $element
 *   The element to validate.
 * @param array $form_state
 *   The current form state.
 * @param array $form
 *   The form structure.
 */
function culturefeed_lists_admin_list_form_validate_list_path(array $element, array &$form_state, array $form) {
  $list_path = $element['#value'];
  $list_edit = $form['#list'];

  // No error when the existing path did not change.
  if ($list_edit && $list_edit->getPath() === $list_path) {
    return;
  }

  // Check if there is already a list with the same path.
  $existing = culturefeed_lists_list_path_load($list_path);
  if (!$existing) {
    return;
  }

  $error = FALSE;

  // Check if there is already an other list with the same path.
  if (!$list_edit || $list_edit->getId() !== $existing->getId()) {
    $error = t('There is already a list with the same path.');
  }

  if ($error) {
    form_error($element, $error);
  }
}

/**
 * Submit handler for culturefeed_lists_admin_list_form().
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 *
 * @see culturefeed_lists_admin_list_form()
 */
function culturefeed_lists_admin_list_form_submit(array $form, array &$form_state) {
  $form_state['redirect'] = culturefeed_lists_admin_path();

  $list = $form['#list'];
  /* @var CulturefeedListsList $list */
  $values = $form_state['values'];
  $list_name = $values['list_name'];
  $list_path = $values['list_path'];
  $message_args = array(
    '%name_new' => $list_name,
    '%path_new' => $list_path,
  );

  // Add a new list item.
  if (empty($list)) {
    $list = CulturefeedListsList::fromName($list_name, $list_path);
    culturefeed_lists_list_save($list);
    drupal_set_message(t('List %name_new added with path %path_new.', $message_args));
    return;
  }

  // Add te existing values to the message args.
  $message_args['%name_old'] = $list->getName();
  $message_args['%path_old'] = $list->getPath();

  $changes = FALSE;

  // Update the existing item only if the name is different.
  if ($list->getName() !== $list_name) {
    $list = $list->rename($list_name);
    drupal_set_message(t('List name %name_old changed to %name_new.', $message_args));
    $changes = TRUE;
  }

  // Update the path only if it has changed.
  if ($list->getPath() !== $list_path) {
    $list = $list->withPath($list_path);
    drupal_set_message(t('List name %name_old changed path from %path_old to %path_new.', $message_args));
    $changes = TRUE;
  }

  if (!$changes) {
    drupal_set_message(t('No changes for list %name_old.', $message_args), 'status');
    return;
  }

  // Save the changes.
  culturefeed_lists_list_save($list);
}

/**
 * Submit handler when the delete button is clicked on the list edit form.
 *
 * Redirects the user to the confirm delete form.
 *
 * @param array $form
 *   Form structure.
 * @param array $form_state
 *   The form state.
 *
 * @see culturefeed_lists_admin_list_form()
 */
function culturefeed_lists_admin_list_form_submit_delete(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  // Redirect to the delete confirmation form.
  $list = $form['#list'];
  /* @var CulturefeedListsList $list */
  $form_state['redirect'] = array(
    culturefeed_lists_admin_path(array($list->getId(), 'delete')),
    array('query' => $destination),
  );
}

/**
 * Submit handler when the cancel button is clicked on the list edit form.
 *
 * Redirects the user default to the lists overview.
 *
 * @see culturefeed_lists_admin_list_form()
 */
function culturefeed_lists_admin_list_form_submit_cancel() {
  $form_state['redirect'] = culturefeed_lists_admin_path();
}

/**
 * Form to confirm the deletion of a list.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 * @param CulturefeedListsList $list
 *   The list to delete.
 *
 * @return array
 *   The form structure.
 *
 * @see culturefeed_lists_admin_list_delete_form_submit()
 */
function culturefeed_lists_admin_list_delete_form(array $form, array &$form_state, CulturefeedListsList $list) {
  $form['#list'] = $list;
  /* @var CulturefeedListsList $list */

  return confirm_form(
    $form,
    t('Delete list %name', array('%name' => $list->getName())),
    culturefeed_lists_admin_path(),
    t('Are you sure that you want to delete this list? This action can not be undone.')
  );
}

/**
 * Submit handler for culturefeed_lists_admin_list_delete_form().
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 *
 * @see culturefeed_lists_admin_list_delete_form()
 */
function culturefeed_lists_admin_list_delete_form_submit(array $form, array &$form_state) {
  $form_state['redirect'] = culturefeed_lists_admin_path();

  $list = $form['#list'];
  /* @var CulturefeedListsList $list */
  $list_name = $list->getName();

  culturefeed_lists_list_delete($list);
  drupal_set_message(
    t('List %name is deleted.', array('%name' => $list_name))
  );
}

/**
 * Manage the events within a list.
 *
 * @param CulturefeedListsList $list
 *   The list to manage.
 *
 * @return array
 *   Render array.
 */
function culturefeed_lists_admin_manage(CulturefeedListsList $list) {
  $display_id = 'default';
  $view = views_get_view('culturefeed_lists_administer');
  $list_id = $list->getId();

  // Set the draggable list name.
  $field = $view->get_item($display_id, 'field', 'draggableviews');
  $field['draggableviews']['CdbItems']['list'] = $list_id;
  $view->set_item($display_id, 'field', 'draggableviews', $field);

  // Set the list_id filter value.
  $filter = $view->get_item($display_id, 'filter', 'list_id');
  $filter['value'] = array($list_id => $list_id);
  $view->set_item($display_id, 'filter', 'list_id', $filter);

  return $view->preview($display_id);
}

/**
 * Confirmation form to remove an item from a list.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 * @param CulturefeedListsList $list
 *   The list to remove the item from.
 * @param ActivityStatsExtendedEntity $event
 *   The event to remove from the list.
 *
 * @return array
 *   Confirmation form structure.
 *
 * @see culturefeed_lists_admin_manage_item_remove_form_submit()
 */
function culturefeed_lists_admin_manage_item_remove_form(array $form, array &$form_state, CulturefeedListsList $list, ActivityStatsExtendedEntity $event) {
  global $language;

  $form['#list'] = $list;
  $form['#event'] = $event;

  $args = array(
    '@list_name' => $list->getName(),
    '@item_name' => $event->getTitle($language->language),
  );

  return confirm_form(
    $form,
    t('Remove item from list'),
    culturefeed_lists_admin_path(array($list->getId(), 'manage')),
    t(
      'Are you sure that you want to remove item <strong>@item_name</strong> from list <strong>@list_name</strong>?',
      $args
    )
  );
}

/**
 * Submit handler for culturefeed_lists_admin_manage_item_remove_form() form.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 *
 * @see culturefeed_lists_admin_manage_item_remove_form()
 */
function culturefeed_lists_admin_manage_item_remove_form_submit(array $form, array &$form_state) {
  global $language;

  $event = $form['#event'];
  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $list = $form['#list'];
  /* @var CulturefeedListsList $list */

  // Update the event.
  try {
    culturefeed_lists_event_remove_list($event->getEntity(), $list);

    $message = t(
      'Item %item_name is removed from list %list_name.',
      array(
        '%item_name' => $event->getTitle($language->language),
        '%list_name' => $list->getName(),
      )
    );
    drupal_set_message($message);

    $form_state['redirect'] = culturefeed_lists_admin_path(
      array($list->getId(), 'manage')
    );
  }
  catch (Exception $e) {
    drupal_set_message(
      t('Error occurred while saving the item lists.'),
      'error'
    );
  }
}

/**
 * Submit handler for the reset button on the management overview view.
 *
 * @see culturefeed_lists_form_views_form_culturefeed_lists_administer_default_alter()
 */
function culturefeed_lists_admin_manage_reset_order_submit(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $list = $form_state['list'];
  /* @var CulturefeedListsList $list */

  // Redirect to the reset confirmation form.
  $form_state['redirect'] = array(
    culturefeed_lists_admin_path(
      array(
        $list->getId(),
        'manage',
        'reset',
      )
    ),
    array('query' => $destination),
  );
}

/**
 * Form to confirm the reset of a list order.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 * @param CulturefeedListsList $list
 *   The list to delete.
 *
 * @return array
 *   The form structure.
 *
 * @see culturefeed_lists_admin_manage_reset_order_form_submit()
 */
function culturefeed_lists_admin_manage_reset_order_form(array $form, array &$form_state, CulturefeedListsList $list) {
  $form['#list'] = $list;

  return confirm_form(
    $form,
    t('Reset the order of %name list', array('%name' => $list->getName())),
    culturefeed_lists_admin_path(array($list->getId(), 'manage')),
    t('Are you sure that you want to reset the order of the list items? This will remove all manually set sorting and can not be undone.')
  );
}

/**
 * Submit handler for the reset of the list order.
 *
 * @param array $form
 *   The form structure.
 * @param array $form_state
 *   The current form state.
 *
 * @see culturefeed_lists_admin_manage_reset_order_form()
 */
function culturefeed_lists_admin_manage_reset_order_form_submit(array $form, array &$form_state) {
  $list = $form['#list'];
  /* @var CulturefeedListsList $list */

  culturefeed_lists_sort_reset($list);
  drupal_set_message(t('Sorting of list is reset.'));
  drupal_goto(culturefeed_lists_admin_path(array($list->getId(), 'manage')));
}
