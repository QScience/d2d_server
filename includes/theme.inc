<?php 
/**
 * @file
 */

/**
 * theme pattern description part
 */
function _d2d_instance_view_page_description($de) {
  $content = '<div id="d2d-instance-view-page-description"><dl class="dl-horizontal">';

  $content .= '<dt>title</dt><dd>' . $de->title . '</dd>';
  $content .= '<dt>description</dt><dd>' . $de->description . '</dd>';

  $category_s = l($de->category, 'pattern/category/' . $de->category);
  $content .= '<dt>category</dt><dd>' . $category_s . '</dd>';

  $content .= '<dt>d2d id</dt><dd>' . $de->d2did. '</dd>';

  $content .= '</dl></div>';
  return $content;
}


