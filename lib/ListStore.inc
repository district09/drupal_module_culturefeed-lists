<?php

/**
 * @file
 * Class to represent the collection of lists.
 */

/**
 * Collection of lists.
 */
class CulturefeedListsListStore extends CulturefeedListsListCollection {

  /**
   * Load the lists when creating the object.
   */
  public function __construct() {
    $lists = variable_get(CULTUREFEED_LISTS_VARIABLE, array());
    parent::__construct($lists);
  }

  /**
   * Add/Update a list item to the list.
   *
   * Adding an existing list will overwrite that list in the collection.
   *
   * @param CulturefeedListsList $list
   *   The list item to add or update.
   */
  public function saveList(CulturefeedListsList $list) {
    $this->lists[$list->getId()] = $list;
    $this->sort();
    $this->save();
  }

  /**
   * Remove a list item from the list.
   *
   * @param CulturefeedListsList $list
   *   The list item to remove.
   *
   * @return bool
   *   Is the list item removed from the list.
   */
  public function removeList(CulturefeedListsList $list) {
    if (!isset($this->lists[$list->getId()])) {
      return FALSE;
    }

    unset($this->lists[$list->getId()]);
    $this->updateKeys();
    $this->rewind();
    $this->save();
    return TRUE;
  }

  /**
   * Save the lists to the variables store.
   */
  protected function save() {
    variable_set(CULTUREFEED_LISTS_VARIABLE, $this->lists);
  }

}
