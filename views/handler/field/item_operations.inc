<?php

/**
 * @file
 * Views field handler to add item operation links.
 */

use \CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * Field handler to add operations to an item in the lists.
 */
class CulturefeedListsViewsHandlerFieldItemOperations extends views_handler_field {

  /**
   * Render the management links.
   *
   * @param ActivityStatsExtendedEntity|mixed $extended_entity
   *   The extended search entity.
   *
   * @return string
   *   The links render.
   */
  public function render($extended_entity) {
    if (!$extended_entity instanceof ActivityStatsExtendedEntity) {
      return '';
    }

    // Only active if user has access to administer lists.
    if (!user_access(CULTUREFEED_LISTS_PERMISSION_ADMINISTER)) {
      return '';
    }

    // Only active on management pages.
    $list = culturefeed_lists_menu_get_list();
    if (!$list) {
      return '';
    }

    $links = array();

    // View the item.
    $links['view'] = array(
      'title' => t('View'),
      'href' => culturefeed_entry_ui_entity_path($extended_entity, 'view'),
    );

    // Manage the item.
    $links['manage'] = array(
      'title' => t('Manage'),
      'href' => culturefeed_entry_ui_entity_path($extended_entity, 'lists'),
    );

    // Remove from the list.
    $links['remove'] = array(
      'title' => t('Remove from list'),
      'href' => culturefeed_lists_admin_path(
        array($list->getId(), 'manage', $extended_entity->getId(), 'remove')
      ),
    );

    return theme(
      'links',
      array(
        'links' => $links,
        'attributes' => array('class' => array('links', 'inline')),
      )
    );
  }

}
