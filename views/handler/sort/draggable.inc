<?php

/**
 * @file
 * View sort handler.
 */

use CultuurNet\Search\Parameter\Sort;

/**
 * Views handler to sort a view by the Cdb Item weight.
 */
class CulturefeedListsViewsHandlerSortDraggable extends views_handler_sort_culturefeed_search_base {

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
    $sort = $this->getSort();
    if (!$sort) {
      return;
    }

    $parameter = new Sort(
      'query({!v="cdbid:(' . implode(' OR ', $sort) . ')"})',
      $this->options['order']
    );
    $this->query->add_parameter($parameter);
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

  /**
   * Create the sort query array.
   *
   * @return array
   *   Array of items to bost the sort for.
   */
  protected function getSort() {
    // Get the list.
    $list = culturefeed_lists_list_load(
      $this->options['list']
    );

    if (!$list) {
      return array();
    }

    // We start the boost low enough.
    $boost = -1000;

    // Loop over the ordered items and add them to the query sort.
    $sort = array();
    $order = array_keys(culturefeed_lists_sort_load($list));
    foreach ($order as $cdbid) {
      $sort[] = sprintf(
        '[%s TO %s]^%d',
        $cdbid,
        $cdbid,
        $boost
      );

      $boost++;
    }

    return $sort;
  }

}
