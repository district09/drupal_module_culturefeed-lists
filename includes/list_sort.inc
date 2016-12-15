<?php

/**
 * @file
 * List Sort API.
 */

/**
 * Get an array with the item uuid's as keys ordered by the sort weight.
 *
 * @param CulturefeedListsList $list
 *   The list to get the sort for.
 *
 * @return array
 *   The sort array keyed by the item uuid's.
 */
function culturefeed_lists_sort_load(CulturefeedListsList $list) {
  $q = db_select('culturefeed_lists_sort', 'cls');
  $q->addField('cls', 'item_id', 'uuid');
  $q->condition('list_id', $list->getId());
  $q->orderBy('weight', 'ASC');

  $uuids = $q->execute()->fetchCol();
  return array_fill_keys($uuids, 0);
}

/**
 * Reset all sorts for the given list.
 *
 * @param CulturefeedListsList $list
 *   The list to get the sort for.
 */
function culturefeed_lists_sort_reset(CulturefeedListsList $list) {
  $q = db_delete('culturefeed_lists_sort');
  $q->condition('list_id', $list->getId());
  $q->execute();
}

/**
 * Add a sort weight for the given list and event.
 *
 * @param CulturefeedListsList $list
 *   The list to add the event weight to.
 * @param string $item_id
 *   The Cdb Item id to set the weight for.
 * @param int $weight
 *   The weight for the sort record.
 */
function culturefeed_lists_sort_add_item_id(CulturefeedListsList $list, $item_id, $weight) {
  // Create new order record.
  $record = array(
    'list_id' => $list->getId(),
    'item_id' => $item_id,
    'weight' => $weight,
  );
  drupal_write_record('culturefeed_lists_sort', $record);
}

/**
 * Remove an event from the list sort.
 *
 * @param CulturefeedListsList $list
 *   The list to remove the event weight from.
 * @param string $item_id
 *   The Cdb Item id remove the weight for.
 */
function culturefeed_lists_sort_remove_item_id(CulturefeedListsList $list, $item_id) {
  db_delete('culturefeed_lists_sort')
    ->condition('list_id', $list->getId())
    ->condition('item_id', $item_id)
    ->execute();
}
