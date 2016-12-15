# CultureFeed Lists
This Drupal module extends the 
[CultureFeed Drupal module suite][link-culturefeed].

It adds the functionality to hand-pick CultureFeed Cdb Items and add them to
curated lists. The items in the lists can be sorted manually.

There is views integration to create overview of Cdb Items that are part of a
list, sorted by the manually set order.



## How it works
The lists are created by adding keywords to the CultureFeed items. These 
keywords are unique for each site (they get a unique id as keyword name). The
actual name is stored in the site (can be renamed without the need to update
all tagged items).

The sorting of the list items is done by keeping a record per list of the 
order of those items. The list is first queried in the CultureFeed search API
and that result ordered in code based on the saved order. Items that are not 
in the sort info (did not get a sort weight) are always put after the manually 
sorted items.

> **NOTE** : Do not put too many items items in a list: ordering too many 
> items in code could slow down managing and showing the sorted lists. 



## Requirements
* [Drupal 7][link-drupal]
* [CultureFeed Drupal module suite][link-culturefeed]
* [Draggableviews][link-draggableviews]



## Installation
* [Install culturefeed module suite][link-culturefeed-install].
* Enable this module (culturefeed_lists).
* Setup the proper permissions for the user role who may manage CultureFeed 
  items and lists.
* Assign the proper role(s) to the user who needs to manage lists.

> **NOTE** : The lists functionality is built upon the CultureFeed Entry API.
> This means that only users who login with an UiT iD account can add items 
> to lists. 



## Usage
* Login with a CultureFeed user (is a requirement to use the CultureFeed 
  Entry API).
* Create one or more lists at Admin > Configuration > CultureFeed > 
  CultureFeed Lists (admin/config/culturefeed/lists).
* Lookup or add events and open the detail page. Depending on the user 
  permissions you get an extra tab "Lists". Select and save the lists the item
  should be listed in.
* You can click on the "Manage list" link to open an overview of the items in
  a list.
* Change the order of the list items by dragging them and save the list.



## Views integration
The CultureFeed Lists adds extra views handler to filter and sort lists of 
items:

* **Filter by List** : Filter the items to only those who are part of one or more
  lists.
* **Sort by list** : Sort the items by the manually set sort (use this in 
  combination with the filter handler).
  
  
  
## Known issues

### Delay after adding/removing items to list(s)
There is a delay between the moment an item is added or removed to a list and
that change being shown on the item lists tab or list management page. 

This is due the fact that changing an item requires that item to be 
re-indexed in the CultureFeed Search API (source for showing items).




[link-drupal]: https://www.drupal.org/project/drupal
[link-culturefeed]: https://github.com/cultuurnet/culturefeed
[link-culturefeed-install]: https://github.com/cultuurnet/culturefeed#install
[link-draggableviews]: https://drupal.org/project/draggableviews
