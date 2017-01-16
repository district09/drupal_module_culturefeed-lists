<?php

/**
 * @file
 * List API.
 */

/**
 * Get info about all available lists.
 *
 * @return CulturefeedListsListStore
 *   Collection of CulturefeedListsList items.
 */
function culturefeed_lists_list_load_all() {
  return new CulturefeedListsListStore();
}

/**
 * Get all the lists as options array (id => name).
 *
 * @return array
 *   Array of list names keyed by their id.
 */
function culturefeed_lists_list_options() {
  $options = array(
    NULL => t('- Choose -'),
  );

  $lists = culturefeed_lists_list_load_all();
  foreach ($lists as $list) {
    $options[$list->getId()] = $list->getName();
  }

  return $options;
}

/**
 * Load a single list item by its id.
 *
 * @param string $id
 *   The list id.
 *
 * @return CulturefeedListsList|false
 *   The found list.
 */
function culturefeed_lists_list_load($id) {
  $lists = culturefeed_lists_list_load_all();

  $lists->seek($id);
  if (!$lists->valid()) {
    return FALSE;
  }

  return $lists->current();
}

/**
 * Save a list.
 *
 * @param CulturefeedListsList $list
 *   The list to save.
 *
 * @return CulturefeedListsList
 *   The updated list.
 */
function culturefeed_lists_list_save(CulturefeedListsList $list) {
  $lists = culturefeed_lists_list_load_all();
  $lists->saveList($list);
  return $list;
}

/**
 * Remove a list item by its id.
 *
 * @param CulturefeedListsList $list
 *   The list item to remove.
 *
 * @return bool
 *   Deleted.
 */
function culturefeed_lists_list_delete(CulturefeedListsList $list) {
  $lists = culturefeed_lists_list_load_all();
  return $lists->removeList($list);
}

/**
 * Helper to convert an array of list items to event keywords.
 *
 * @param CulturefeedListsListCollection $lists
 *   List items collection or array of lists.
 *
 * @return CultureFeed_Cdb_Data_Keyword[]
 *   Array of keyword objects.
 */
function culturefeed_lists_lists_to_keywords(CulturefeedListsListCollection $lists) {
  $keywords = array();

  foreach ($lists as $list) {
    $id = $list->getId();
    $keywords[$id] = new CultureFeed_Cdb_Data_Keyword($id, FALSE);
  }

  return $keywords;
}

/**
 * Helper to get the current list from the current menu item.
 *
 * @param string $path
 *   Optional path to search for.
 *
 * @return CulturefeedListsList|false
 *   The current list (if any).
 */
function culturefeed_lists_menu_get_list($path = NULL) {
  $item = menu_get_item($path);
  if (!$item) {
    return FALSE;
  }

  $load_function = reset($item['load_functions']);
  if ($load_function !== 'culturefeed_lists_list_load') {
    return FALSE;
  }

  return reset($item['page_arguments']);
}

/**
 * Load a single list item by its path.
 *
 * @param string $path
 *   The list path.
 *
 * @return CulturefeedListsList|false
 *   The found list.
 */
function culturefeed_lists_list_path_load($path) {
  $lists = culturefeed_lists_list_load_all();
  $lists->seekByPath($path);
  if (!$lists->valid()) {
    return FALSE;
  }

  return $lists->current();
}

/**
 * Check if a List path is already in use.
 *
 * @param string $path
 *   The path to check.
 *
 * @return bool
 *   Exists.
 */
function culturefeed_lists_list_path_exists($path) {
  return (bool) culturefeed_lists_list_path_load($path);
}
