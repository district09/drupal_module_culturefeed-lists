<?php

/**
 * @file
 * Filter the events by list name.
 */

use \CultuurNet\Search\Parameter\FilterQuery;

/**
 * Views Handler to filter event lists by a given list.
 */
class CulturefeedListsViewsHandlerFilterListId extends views_handler_filter {
  /**
   * The list options.
   *
   * @var array
   */
  protected $listOptions;

  /**
   * Helper to get the lists.
   *
   * @return array
   *   The list items.
   */
  public function getListOptions() {
    if (!is_array($this->listOptions)) {
      $this->listOptions = culturefeed_lists_list_options();
    }

    return $this->listOptions;
  }

  /**
   * Get the value.
   *
   * @return array
   *   Selected lists.
   */
  public function getValue() {
    if (empty($this->value)) {
      return array();
    }

    if (!is_array($this->value)) {
      return array($this->value);
    }

    return $this->value;
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function options_form(&$form, &$form_state) {
    // @codingStandardsIgnoreEnd
    parent::options_form($form, $form_state);

    $form['value'] = array(
      '#type' => 'checkboxes',
      '#title' => t('List'),
      '#description' => t('Select one or more lists the item is part of.'),
      '#options' => $this->getListOptions(),
      '#default_value' => $this->getValue(),
    );
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function options_submit(&$form, &$form_state) {
  // @codingStandardsIgnoreEnd
    $form_state['values']['options']['value'] = array_filter(
      $form_state['values']['options']['value']
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    if (empty($this->value)) {
      return;
    }

    $keywords = sprintf(
      '(keywords:"%s")',
      implode('" OR "', $this->value)
    );

    $parameter = new FilterQuery($keywords);
    $this->query->add_parameter($parameter);
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function admin_summary() {
  // @codingStandardsIgnoreEnd
    return implode(', ', $this->getValue());
  }

}
