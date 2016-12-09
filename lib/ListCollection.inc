<?php

/**
 * @file
 * Class to represent the collection of lists.
 */

/**
 * Collection of lists.
 */
class CulturefeedListsListCollection implements Countable, SeekableIterator {
  /**
   * The lists as loaded from the variable store.
   *
   * @var CulturefeedListsList[]
   */
  protected $lists;

  /**
   * The list keys.
   *
   * @var array
   */
  protected $keys;

  /**
   * The current iterator position.
   *
   * @var int
   */
  protected $position = 0;

  /**
   * Create the object by passing an array of lists.
   *
   * @param CulturefeedListsList[] $lists
   *   The lists to store in the collection.
   */
  public function __construct(array $lists) {
    $this->lists = $lists;
    $this->sort();
  }

  /**
   * Get the list as an array.
   *
   * @return CulturefeedListsList[]
   *   Array of CulturefeedListsList items keyed by their id.
   */
  public function toArray() {
    return $this->lists;
  }

  /**
   * Get the elements that are in this collection but not in the other.
   *
   * @param CulturefeedListsListCollection $compare
   *   The list to compare with.
   *
   * @return CulturefeedListsListCollection
   *   The items in this list but not in the other.
   */
  public function diff(CulturefeedListsListCollection $compare) {
    $diff = array_diff_key($this->toArray(), $compare->toArray());
    return new CulturefeedListsListCollection($diff);
  }

  /**
   * Get the elements that are in this collection and also in the other.
   *
   * @param CulturefeedListsListCollection $compare
   *   The list to compare with.
   *
   * @return CulturefeedListsListCollection
   *   The items in this list but not in the other.
   */
  public function intersect(CulturefeedListsListCollection $compare) {
    $intersect = array_intersect_key($this->toArray(), $compare->toArray());
    return new CulturefeedListsListCollection($intersect);
  }

  /**
   * Check if a given list is in the collection.
   *
   * @param CulturefeedListsList $list
   *   The list to check if it is in the list.
   *
   * @return bool
   *   In list.
   */
  public function hasList(CulturefeedListsList $list) {
    return !empty($this->lists[$list->getId()]);
  }

  /**
   * Get the current item from the collection (if any).
   *
   * @return CulturefeedListsList|null
   *   The list item (if any).
   */
  public function current() {
    if (!$this->valid()) {
      return NULL;
    }

    return $this->lists[$this->key()];
  }

  /**
   * {@inheritdoc}
   */
  public function next() {
    $this->position++;
  }

  /**
   * {@inheritdoc}
   */
  public function key() {
    if (!$this->valid()) {
      return NULL;
    }

    return $this->keys[$this->position];
  }

  /**
   * {@inheritdoc}
   */
  public function valid() {
    if ($this->position === FALSE) {
      return FALSE;
    }

    return !empty($this->keys[$this->position]);
  }

  /**
   * {@inheritdoc}
   */
  public function rewind() {
    $this->position = 0;
  }

  /**
   * {@inheritdoc}
   */
  public function count() {
    return count($this->lists);
  }

  /**
   * Move the collection pointer to the given list id.
   *
   * @param string $id
   *   The list id.
   */
  public function seek($id) {
    $this->position = array_search($id, $this->keys, TRUE);
  }

  /**
   * Helper to update the key positions when the list changes.
   */
  protected function updateKeys() {
    $this->keys = array_keys($this->lists);
  }

  /**
   * Helper to sort the lists by their name.
   */
  protected function sort() {
    uasort($this->lists, function (CulturefeedListsList $a, CulturefeedListsList $b) {
      return strcasecmp($a->getName(), $b->getName());
    });
    $this->updateKeys();
    $this->rewind();
  }

}
