<?php

/**
 * @file
 * View handler field sort.
 */

/**
 * View handler to add the draggable views field to a Culturefeed view.
 */
class CulturefeedListsViewsHandlerFieldDraggable extends draggableviews_handler_field_draggable {

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function options_form(&$form, &$form_state) {
  // @codingStandardsIgnoreEnd
    parent::options_form($form, $form_state);

    // Remove hierarchy options.
    $form['draggableviews']['handler']['#value'] = 'CdbItems';
    $form['draggableviews']['handler']['#default_value'] = 'CdbItems';
    $form['draggableviews']['handler']['#disabled'] = TRUE;
    $form['draggableviews']['hierarchy_handler']['#access'] = FALSE;
  }

  /**
   * {@inheritdoc}
   *
   * This overrides the parent method so we can use the Culturefeed entity
   * UUID instead of Drupal entity record ID.
   */
  // @codingStandardsIgnoreStart
  public function views_form(&$form, &$form_state) {
  // @codingStandardsIgnoreEnd
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    // If this view is a summary, do not add a form.
    if ($this->field_alias === 'unknown') {
      return;
    }

    // Add the list to the view.
    $list_id = $this->options['draggableviews']['CdbItems']['list'];
    $list = culturefeed_lists_list_load($list_id);
    $form_state['list'] = $list;

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );
    $range = count($this->view->result);

    // At this point, the query has already been run, so we can access the
    // results in order to get the base key value (for example, nid for
    // nodes).
    foreach ($this->view->result as $row_index => $row) {
      $entity = $this->view->result[$row_index];
      /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $entity */
      $entity_id = $entity->getId();

      $form[$this->options['id']][$row_index] = array(
        '#tree' => TRUE,
      );

      $handler_object = draggableviews_get_handler_class($this->options['draggableviews']['handler']);
      /* @var draggableviews_handler $handler_object */

      // Weight field selectbox.
      $form[$this->options['id']][$row_index]['weight'] = array(
        '#type' => 'select',
        '#options' => range(-$range, $range),
        '#attributes' => array('class' => array('draggableviews-weight')),
        '#default_value' => $handler_object->get($this, $row_index),
      );

      // Item to keep id of the entity.
      $form[$this->options['id']][$row_index]['id'] = array(
        '#type' => 'hidden',
        '#value' => $entity_id,
        '#attributes' => array('class' => 'draggableviews-id'),
      );
    }
  }

}
