<?php

/**
 * @file
 * View sort handler.
 */

/**
 * Views handler to sort a view by the Cdb Item weight.
 */
class CulturefeedListsViewsHandlerSortDraggable extends views_handler {

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function can_expose() {
    // @codingStandardsIgnoreEnd
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function option_definition() {
    // @codingStandardsIgnoreEnd
    $options = parent::option_definition();
    $options['list'] = array('list' => NULL);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function options_form(&$form, &$form_state) {
    // @codingStandardsIgnoreEnd
    parent::options_form($form, $form_state);

    // Lists to select the sort order from.
    $options = culturefeed_lists_list_options();

    // Add warnings when there are no lists defined.
    if (!count($options)) {
      $form['list_warning'] = array(
        '#markup' => t('There are no lists defined. Add one or more lists so they can be selected.'),
        '#prefix' => '<div class="messages warning">',
        '#postfix' => '</div>',
      );
    }

    $form['list'] = array(
      '#type' => 'select',
      '#title' => t('List'),
      '#description' => t('The list to sort the items by.'),
      '#options' => $options,
      '#default_value' => $this->options['list'],
      '#empty_option' => t('- Choose -'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // No query required.
    // The actual ordering will be performed by self::post_execute().
  }

  /**
   * {@inheritdoc}
   *
   * Sorts the view items by the draggable views list items weight.
   */
  // @codingStandardsIgnoreStart
  public function post_execute(&$values) {
    // @codingStandardsIgnoreEnd
    if (empty($this->options['list'])) {
      return;
    }

    $list = culturefeed_lists_list_load(
      $this->options['list']
    );
    if (!$list) {
      return;
    }

    _culturefeed_lists_view_values_sort($values, $list);
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function admin_summary() {
    // @codingStandardsIgnoreEnd
    if (empty($this->options['list'])) {
      return t('No list selected.');
    }

    $lists = culturefeed_lists_list_load_all();
    $lists->seek($this->options['list']);
    if (!$lists->valid()) {
      return t('No list selected.');
    }

    $list = $lists->current();
    return t(
      'Order items by list %list.',
      array('%list' => $list->getName())
    );
  }

}
