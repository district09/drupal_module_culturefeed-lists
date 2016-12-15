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
    'help' => t('Filter the items by the list(s) they are part of.'),
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
