<?php

/**
 * @file
 * Views related hooks and functionality.
 */

/**
 * Implements hook_views_data_alter().
 */
function culturefeed_lists_views_data_alter(&$data) {
  if (!isset($data['cdb_items'])) {
    return;
  }

  $group = t('Cdb Items Lists');

  // Filter items by list keywords.
  $data['cdb_items']['list_id'] = array(
    'title' => t('List'),
    'help' => t('The lists an item is part of.'),
    'group' => $group,
    'real field' => 'cdbid',
    'filter' => array(
      'label' => t('List'),
      'handler' => 'CulturefeedListsViewsHandlerFilterListId',
      'click sortable' => FALSE,
    ),
  );

  // List operations.
  $data['cdb_items']['list_item_operations'] = array(
    'title' => t('Item operations'),
    'help' => t('Operational links to manage a list item.'),
    'group' => $group,
    'real field' => 'cdbid',
    'field' => array(
      'label' => t('Operations'),
      'handler' => 'CulturefeedListsViewsHandlerFieldItemOperations',
    ),
  );

  // Draggable views integration to sort list items.
  $data['cdb_items']['draggableviews'] = array(
    'title' => t('Draggableviews'),
    'help' => t('Provides functionality to alter the order of Cdb items in lists.'),
    'group' => $group,
    'real field' => 'cdbid',
    'field' => array(
      'label' => t('Draggableviews order'),
      'handler' => 'CulturefeedListsViewsHandlerFieldDraggable',
      'click sortable' => FALSE,
    ),
    'sort' => array(
      'label' => t('Draggableviews order'),
      'help' => t('Sort Cdb item lists by the sort weight.'),
      'handler' => 'CulturefeedListsViewsHandlerSortDraggable',
    ),
  );
}

/**
 * Implements hook_views_pre_render().
 *
 * @see culturefeed_lists_views_pre_render_reset()
 * @see culturefeed_lists_views_pre_render_sort()
 */
function culturefeed_lists_views_pre_render(view $view) {
  $list = culturefeed_lists_views_extract_list($view);
  if (!$list) {
    return;
  }

  // Are there ordered items?
  $order = culturefeed_lists_sort_load($list);
  if (empty($order)) {
    return;
  }

  // Sort the query result.
  $items = $view->result;
  /* @var \CultuurNet\Search\ActivityStatsExtendedEntity[] $items */
  foreach ($items as $key => $item) {
    $item_id = $item->getId();

    if (!isset($order[$item_id])) {
      continue;
    }

    $order[$item_id] = $item;
    unset($items[$key]);
  }

  // Filter out items without items.
  $order = array_filter($order);

  // Combine sorted and unsorted items.
  $view->result = array_merge(
    array_values($order),
    array_values($items)
  );
}

/**
 * Helper to get the list to load the order for from a view.
 *
 * @param view $view
 *   The view to load the list for.
 *
 * @return CulturefeedListsList|false
 *   The list (if any).
 */
function culturefeed_lists_views_extract_list(view $view) {
  if ($view->base_table !== 'cdb_items') {
    return FALSE;
  }

  // Extract from the draggable views field.
  $list = culturefeed_lists_views_extract_list_from_field_draggable($view);
  if ($list) {
    return $list;
  }

  // Extract from the sort field.
  $list = culturefeed_lists_views_extract_list_from_filter_list_id($view);
  if ($list) {
    return $list;
  }

  return FALSE;
}

/**
 * Helper to extract the list from the draggable field.
 *
 * @param view $view
 *   The view to extract from.
 *
 * @return CulturefeedListsList|false
 *   The list (if any).
 */
function culturefeed_lists_views_extract_list_from_field_draggable(view $view) {
  $field = $view->get_item('default', 'field', 'draggableviews');
  if (!$field) {
    return FALSE;
  }

  if (empty($field['draggableviews']['CdbItems']['list'])) {
    return FALSE;
  }

  $list_id = $field['draggableviews']['CdbItems']['list'];
  return culturefeed_lists_list_load($list_id);
}

/**
 * Helper to extract the list from the draggable sort.
 *
 * @param view $view
 *   The view to extract from.
 *
 * @return CulturefeedListsList|false
 *   The list (if any).
 */
function culturefeed_lists_views_extract_list_from_filter_list_id(view $view) {
  $filter = $view->get_item('default', 'filter', 'list_id');
  if (!$filter) {
    return FALSE;
  }

  if (empty($filter['value'])) {
    return FALSE;
  }

  $list_id = array_shift($filter['value']);
  return culturefeed_lists_list_load($list_id);
}
