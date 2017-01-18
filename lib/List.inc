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
   * The list path.
   *
   * @var string
   */
  private $path;

  /**
   * List should be created with a named constructor.
   *
   * @see CulturefeedListsList::fromName()
   * @see CulturefeedListsList::fromArray()
   */
  private function __construct() {
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
   * Get the path.
   *
   * @return string
   *   The path.
   */
  public function getPath() {
    return $this->path;
  }

  /**
   * Change the path.
   *
   * @param string $path
   *   The new path.
   *
   * @return CulturefeedListsList
   *   A new object containing the new path.
   */
  public function withPath($path) {
    $list = clone $this;
    $list->path = $path;

    return $list;
  }

  /**
   * Get the array notation for a list.
   *
   * @return array
   *   Array containing:
   *   - id: The list id.
   *   - name: The list name.
   */
  public function toArray() {
    return array(
      'id' => $this->getId(),
      'name' => $this->getName(),
      'path' => $this->getPath(),
    );
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

  /**
   * Create a list item from the given name.
   *
   * @param string $name
   *   The list name.
   * @param string $path
   *   Optional list path, will auto generate a path if none is given.
   *
   * @return CulturefeedListsList
   *   The created list object.
   *
   * @throws InvalidArgumentException
   *   When the name is empty.
   */
  static public function fromName($name, $path = NULL) {
    $name = trim($name);
    if (empty($name)) {
      throw new InvalidArgumentException('The list name can not be empty.');
    }

    $list = new static();
    $list->createId();
    $list->name = $name;

    if (empty($path)) {
      $path = $name;
    }
    $list->path = preg_replace('/[^a-z0-9-]+/', '-', strtolower($path));

    return $list;
  }

  /**
   * Create a list item from a given array.
   *
   * @param string $id
   *   The list id.
   * @param string $name
   *   The list name.
   * @param string $path
   *   Optional list path, will auto generate a path if none is given.
   *
   * @return CulturefeedListsList
   *   The created list object.
   *
   * @throws InvalidArgumentException
   *   When the id is not valid.
   */
  static public function fromIdAndName($id, $name, $path = NULL) {
    $id = trim($id);
    if (empty($id)) {
      throw new InvalidArgumentException('The list id can not be empty.');
    }

    $list = static::fromName($name, $path);
    $list->id = $id;

    return $list;
  }

}
