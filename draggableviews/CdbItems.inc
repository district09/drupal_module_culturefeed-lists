<?php

/**
 * @file
 * Native handler plugin.
 */

$plugin = array(
  'label' => 'Cdb Items',
  'handler' => array(
    'class' => 'CulturefeedListsDraggableviewsCdbItems',
  ),
);

/**
 * Custom draggableviews handler to save and get the sort order.
 */
class CulturefeedListsDraggableviewsCdbItems extends draggableviews_handler_native {

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function option_definition() {
  // @codingStandardsIgnoreEnd
    return array(
      'list' => '',
    );
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function options_form($field) {
  // @codingStandardsIgnoreEnd
    $form = array();

    // Define the list options.
    $options = culturefeed_lists_list_options();

    // Add warnings when there are no lists defined.
    if (count($options) < 2) {
      $form['list_warning'] = array(
        '#markup' => t('There are no lists defined. Add one or more lists so they can be selected.'),
        '#prefix' => '<div class="messages warning">',
        '#postfix' => '</div>',
      );
    }

    $default = $field->options['draggableviews']['CdbItems']['list'];
    $form['list'] = array(
      '#type' => 'select',
      '#title' => t('List'),
      '#options' => $options,
      '#default_value' => $default,
      '#description' => t('Please select the list to store the items sorting for.'),
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function set($form_state) {
    $items = $form_state['values']['draggableviews'];
    /* @var array $items */
    $list = $form_state['list'];
    /* @var CulturefeedListsList $list */

    $weight = 0;
    foreach ($items as $item) {
      // Make sure id is available.
      if (!isset($item['id'])) {
        continue;
      }

      // Delete previous order record.
      culturefeed_lists_sort_remove_item_id($list, $item['id']);

      // Create new order record.
      culturefeed_lists_sort_add_item_id($list, $item['id'], $weight);

      $weight++;
    }

    drupal_set_message(t('Order of the items is saved.'));
  }

}
