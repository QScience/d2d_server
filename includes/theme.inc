<?php 
/**
 * @file
 */

/**
 * theme d2d instance description part
 */
function _d2d_instance_view_page_description($de) {
  $content = '<div id="d2d-instance-view-page-description"><dl class="dl-horizontal">';

  $content .= '<dt>title</dt><dd>' . $de->title . '</dd>';
  $content .= '<dt>description</dt><dd>' . $de->description . '</dd>';

  $category_s = l($de->category, 'd2d/category/' . $de->category);
  $content .= '<dt>category</dt><dd>' . $category_s . '</dd>';

  $content .= '<dt>d2d id</dt><dd>' . $de->d2did. '</dd>';

  $content .= '</dl></div>';
  return $content;
}

/**
 * theme d2d instances list page.
 */
function _d2d_server_list_page($query, $fieldset_title) {
  //drupal_add_css(drupal_get_path('module', 'patterns_server') . '/styles/css/patterns_server.table.css');
  drupal_add_js("misc/autocomplete.js");
  //drupal_add_js(drupal_get_path('module', 'patterns_server') . '/styles/js/patterns_server.table.js');

  $header = array(
    t('Category'),
    t('Title'),
    t('D2DID'),
    t('Created'),
  );
  $ids = array();
  foreach ($query->ordered_results as $value) {
    $ids[] = $value->entity_id;
  }
  $entities = entity_load('d2d_entity', $ids);
  $rows = array();
  foreach ($entities as $ety_obj) {
    $rows[] = _d2d_server_list_table_row($ety_obj);
  }
  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('d2d-server-list-table'))));
  if (isset($query->pager['element'])) {
    $static_pager_element = $query->pager['element'];
  }
  else {
    $static_pager_element = 0;
  }
  $html .= theme('pager', array('tags' => array(), 'element' => $static_pager_element++));

  //$upload_button = _patterns_server_upload_button();
  $div_b = "<div class='d2d-server-list-table-wrap'><fieldset><legend>$fieldset_title</legend><div class='fieldset-wrapper'>";
  //$div_b = "<div class='patterns-server-list-table-wrap'><fieldset><legend>$fieldset_title$upload_button</legend><div class='fieldset-wrapper'>";
  $div_e = '</div></fieldset></div>';

  return $div_b . $html . $div_e;
}

/**
 * generate d2d instances list table row.
 * used by _d2d_server_list_page($query).
 */
function _d2d_server_list_table_row($ety_obj) {
  $row = array();

  $row[] = l($ety_obj->category, 'd2d/category/' . $ety_obj->category);
  $row[] = l($ety_obj->title, 'd2d/' . $ety_obj->did);
  $row[] = $ety_obj->d2did;
  $row[] = '<i value="' . $ety_obj->updated . '" ' . 'class="upload-time">' . date('j-m-Y H:i:s (P)', $ety_obj->updated) . '</i>';
  return $row;
}
