<?php

/**
 * @file
 * List Event API.
 */

/**
 * Get the lists for the given event entity.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event to get the lists for.
 *
 * @return CulturefeedListsListCollection
 *   The list collection for the event.
 */
function culturefeed_lists_event_load_lists(\CultureFeed_Cdb_Item_Event $event) {
  $lists = culturefeed_lists_list_load_all()->toArray();
  $keywords = $event->getKeywords(TRUE);

  return new CulturefeedListsListCollection(
    array_intersect_key($lists, $keywords)
  );
}

/**
 * Update the lists for an event.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event to get the lists for.
 * @param CulturefeedListsListCollection $lists
 *   The lists to update the event with.
 *
 * @throws Exception
 */
function culturefeed_lists_event_update_lists(\CultureFeed_Cdb_Item_Event $event, CulturefeedListsListCollection $lists) {
  try {
    // Lists currently linked to to the event.
    $event_lists = culturefeed_lists_event_load_lists($event);

    // Lists to add.
    $added_lists = $lists->diff($event_lists);
    if ($added_lists->count()) {
      Drupalculturefeed_EntryApi::addTagToEvent(
        $event,
        culturefeed_lists_lists_to_keywords($added_lists)
      );
    }

    // Lists to remove.
    $removed_lists = $event_lists->diff($lists);
    if ($removed_lists->count()) {
      foreach ($removed_lists as $list) {
        Drupalculturefeed_EntryApi::removeTagFromEvent($event, $list->getId());
      }
    }

    cache_clear_all(
      'culturefeed:results:detail:event:' . $event->getCdbId(),
      'cache_culturefeed_search'
    );
  }
  catch (Exception $e) {
    // Log the exception and throw it further up.
    watchdog_exception('Culturefeed List', $e);
    throw $e;
  }
}

/**
 * Remove an event from a list.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event to remove from the list.
 * @param CulturefeedListsList $list
 *   The list to remove the item from.
 */
function culturefeed_lists_event_remove_list(\CultureFeed_Cdb_Item_Event $event, CulturefeedListsList $list) {
  $event_lists = culturefeed_lists_event_load_lists($event);

  // Only update the event if it has the list to remove.
  if (!$event_lists->hasList($list)) {
    return;
  }

  $remove = new CulturefeedListsListCollection(array($list->getId() => $list));
  $new_event_lists = $event_lists->diff($remove);
  culturefeed_lists_event_update_lists($event, $new_event_lists);
}
