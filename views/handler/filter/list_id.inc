<?php

/**
 * @file
 * Filter the events by list name.
 */

/**
 * Views Handler to filter event lists by a given list.
 */
class CulturefeedListsViewsHandlerFilterListId extends views_handler_filter_culturefeed_search_in {
  /**
   * The list options.
   *
   * @var array
   */
  protected $listOptions;

  /**
   * {@inheritdoc}
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->search_field = 'keywords';
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function value_form(&$form, &$form_state) {
    // @codingStandardsIgnoreEnd
    $form['value'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Type'),
      '#default_value' => $this->value,
      '#options' => $this->get_value_options(),
    );
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function get_value_options() {
    // @codingStandardsIgnoreEnd
    if (!is_array($this->listOptions)) {
      $this->listOptions = culturefeed_lists_list_options();
    }

    return $this->listOptions;
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function admin_summary() {
    // @codingStandardsIgnoreEnd
    if (empty($this->value)) {
      return;
    }

    $value = is_array($this->value)
      ? $this->value
      : array($this->value);

    $lists = array();
    foreach ($value as $list_id) {
      $list = culturefeed_lists_list_load($list_id);
      $lists[] = $list->getName();
    }

    return implode(', ', $lists);
  }

}
