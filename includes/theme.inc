<?php
/**
 * @file
 */

module_load_include('inc', 'd2d_server', 'includes/votingapi');

/**
 * theme d2d instance description part
 */
function _d2d_instance_view_page_description($de) {
  $content = '<div id="d2d-instance-view-page-description"><dl class="dl-horizontal">';

  $content .= '<dt>d2d instance name</dt><dd>' . $de->title . '</dd>';
  //$name = l($de->site_name, $url);
  $url = substr($de->url, 0, -10);
  $name = l($de->site_name, $url);
  $content .= '<dt>website name</dt><dd>' . $name . '</dd>';
  if (!empty($de->site_slogan)) {
    $content .= '<dt>website slogan</dt><dd>' . $de->site_slogan . '</dd>';
  }
  $content .= '<dt>d2d id</dt><dd>' . $de->d2did . '</dd>';
  $content .= '<dt>description</dt><dd>' . $de->description . '</dd>';


  $category_s = l($de->category, 'd2d/category/' . $de->category);
  $content .= '<dt>category</dt><dd>' . $category_s . '</dd>';

  $score = _d2d_server_get_voting_score($de->did);
  $content .= '<dt>score</dt><dd id="d2d-server-view-page-score">' . $score . '</dd>';
  $content .= '<dt>users number</dt><dd>' . $de->users_number . '</dd>';
  $content .= '<dt>nodes number</dt><dd>' . $de->nodes_number . '</dd>';

  $content .= '</dl></div>';
  return $content;
}

/**
 * theme d2d instances list page.
 */
function _d2d_server_list_page($query, $fieldset_title) {
  drupal_add_css(drupal_get_path('module', 'd2d_server') . '/styles/css/table.css');
  drupal_add_js("misc/autocomplete.js");
  drupal_add_js(drupal_get_path('module', 'd2d_server') . '/styles/js/table.js');

  $header = array(
    t('Category'),
    t('Name'),
    t('D2DID'),
    t('Updated'),
    t('Score'),
    t('Like'),
  );
  $ids = array();
  foreach ($query->ordered_results as $value) {
    $ids[] = $value->entity_id;
  }
  $entities = entity_load('d2d_entity', $ids);
  $field_names = _d2d_server_get_all_taxonomyfields_name('d2d_entity');
  $rows = array();
  foreach ($entities as $ety_obj) {
    $rows[] = _d2d_server_list_table_row($ety_obj, $field_names);
  }
  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('d2d-server-list-table'))));
  if (isset($query->pager['element'])) {
    $static_pager_element = $query->pager['element'];
  }
  else {
    $static_pager_element = 0;
  }
  $html .= theme('pager', array('tags' => array(), 'element' => $static_pager_element++));

  $div_b = "<div class='d2d-server-list-table-wrap'><fieldset><legend>$fieldset_title</legend><div class='fieldset-wrapper'>";
  $div_e = '</div></fieldset></div>';

  return $div_b . $html . $div_e;
}

/**
 * generate d2d instances list table row.
 * used by _d2d_server_list_page($query).
 */
function _d2d_server_list_table_row($ety_obj, $field_names) {
  dsm($ety_obj);
  dsm($field_names);
  $row = array();

  $row[] = l($ety_obj->category, 'd2d/category/' . $ety_obj->category);
  $row[] = l($ety_obj->title, 'd2d/' . $ety_obj->did);

  foreach ($field_names as $field_name) {
    $field = $ety_obj->$field_name;
    $cn = count($field);
    if ($cn > 0) {
      foreach ($field['und'] as $key => $value) {

      }
    }
  }
  $row[] = $ety_obj->d2did;

  $row[] = '<i value="' . $ety_obj->updated . '" ' . 'class="import-time">' . date('j-m-Y H:i:s (P)', $ety_obj->updated) . '</i>';
  $row[] = _d2d_server_get_voting_score($ety_obj->did);
  $voted = _d2d_server_check_whether_voted($ety_obj->did);
  if ($voted) {
    $row['d2d_server_voting_link'] = '<strong class="d2d-server-voted-sign">voted</strong>';
  }
  else {
    $row['d2d_server_voting_link'] = l(t('voting'), 'd2d/' . $ety_obj->did . '/voting', array('attributes' => array('class' => "d2d-server-voting-link d2d-server-voting-link-$ety_obj->did", 'title' => "d2d-server-voting-link-$ety_obj->did")));
  }
  return $row;
}

/**
 * backtoserver button link and like lint in the view page.
 */
function _d2d_server_back_like_button($did) {
  $url = 'd2d_server_homepage';
  $backtoserver_link = l(t('Back To Server'), $url, array('attributes' => array('class' => array('backtoserver-button-link'))));
  $backtoserver_link = "<strong>$backtoserver_link</strong>";

  $voted = _d2d_server_check_whether_voted($did);
  if ($voted) {
    $vote_s = '<strong class="d2d-server-voted-sign" id="d2d-server-view-page-vote">voted</strong>';
  }
  else {
    $url = 'd2d/' . $did . '/voting';
    $vote_s = l(t('voting'), $url, array('attributes' => array('class' => array('d2d-server-voting-link', "d2d-server-voting-link-$did"), 'title' => "d2d-server-voting-link-$did", 'id' => array('d2d-server-view-page-vote'))));
  }

  return $backtoserver_link . $vote_s;
}


