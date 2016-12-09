<?php

/**
 * @file
 * Class representing a list.
 */

/**
 * Object representing a list.
 */
class CulturefeedListsList {
  /**
   * The list id.
   *
   * @var string
   */
  private $id;

  /**
   * The list name.
   *
   * @var string
   */
  private $name;

  /**
   * Create a new list by giving it a name.
   *
   * @param string $name
   *   The list name.
   */
  public function __construct($name) {
    $this->createId();
    $this->name = $name;
  }

  /**
   * Get the list id.
   *
   * @return string
   *   The list id.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Get the list name.
   *
   * @return string
   *   The list name.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Rename a list.
   *
   * @param string $name
   *   The new name for the list.
   *
   * @return CulturefeedListsList
   *   A new object containing the new list name.
   */
  public function rename($name) {
    $list = clone $this;
    $list->name = $name;

    return $list;
  }

  /**
   * Create a new id for the list.
   */
  protected function createId() {
    $prefix = 'list-'
      . variable_get('culturefeed_lists_id_prefix', '0123456789abc')
      . '-';

    $this->id = uniqid($prefix);
  }

}
