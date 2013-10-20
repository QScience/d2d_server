<?php
/**
 * @file
 */

module_load_include('inc', 'd2d_server', 'includes/votingapi');

/**
 * theme d2d instance description part
 */
function _d2d_instance_view_page_description($de, $friends = FALSE) {
  $content = '<div id="d2d-instance-view-page-description"><dl class="dl-horizontal">';

  $content .= '<dt>Name:</dt><dd>' . $de->title . '</dd>';
  //$name = l($de->site_name, $url);
  $url = substr($de->url, 0, -10);
  $name = l($url, $url);
  $content .= '<dt>Website:</dt><dd>' . $name . '</dd>';
  if (!empty($de->site_slogan)) {
    $content .= '<dt>Website slogan:</dt><dd>' . $de->site_slogan . '</dd>';
  }
  $content .= '<dt>D2D id:</dt><dd>' . $de->d2did . '</dd>';

  $category_s = l($de->category, 'd2d/category/' . $de->category);
  $content .= '<dt>Category:</dt><dd>' . $category_s . '</dd>';
  $content .= '<dt>Description:</dt><dd id="viewpage-category-dldtdd">' . $de->description . '</dd>';

  $score = _d2d_server_get_voting_score($de->did);
  $content .= '<dt>Score:</dt><dd id="d2d-server-view-page-score">' . $score . '</dd>';
  $content .= '<dt>Users:</dt><dd>' . $de->users_number . '</dd>';
  $content .= '<dt>Nodes:</dt><dd>' . $de->nodes_number . '</dd>';
  if ($friends != FALSE) {
    $content .= '<dt>Friends:</dt><dd>' . count($friends) . '</dd>';
  }

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
    t('Tags'),
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
  $row = array();

  $row[] = l($ety_obj->category, 'd2d/category/' . $ety_obj->category);
  $row[] = l($ety_obj->title, 'd2d/' . $ety_obj->d2did);

  $tag_row = NULL;
  if ($field_names === FALSE) {
    $tag_row .= t("no valid tag");
  }
  else { 
    foreach ($field_names as $field_name) {
      $field = $ety_obj->$field_name;
      $cn = count($field);
      if ($cn > 0) {
        foreach ($field['und'] as $key => $value) {
          $t_obj = taxonomy_term_load($value['tid']);
          $tag_row .= l($t_obj->name, 'd2d/tags/' . $field_name . '/' . $value['tid']) . ' ';
        }
      }
      else {
        $tag_row .= t("no valid tag");
      }
    }
  }
  $row[] = $tag_row;
  //$row[] = $ety_obj->d2did;

  $row[] = '<i value="' . $ety_obj->updated . '" ' . 'class="import-time">' . date('j-m-Y H:i:s (P)', $ety_obj->updated) . '</i>';
  $row[] = _d2d_server_get_voting_score($ety_obj->did);
  $voted = _d2d_server_check_whether_voted($ety_obj->did);
  if ($voted === 1) {
    $row['d2d_server_voting_link'] = l(t('undovote'), 'd2d/' . $ety_obj->d2did . '/undovote', array('attributes' => array('class' => "d2d-server-undovote-link d2d-server-undovote-link-$ety_obj->did", 'title' => "d2d-server-undovote-link-$ety_obj->did")));
  }
  elseif ($voted === 0) {
    $row['d2d_server_voting_link'] = l(t('voting'), 'd2d/' . $ety_obj->d2did . '/voting', array('attributes' => array('class' => "d2d-server-voting-link d2d-server-voting-link-$ety_obj->did", 'title' => "d2d-server-voting-link-$ety_obj->did")));
  }
  else {
    //$row['d2d_server_voting_link'] = '<strong class="d2d-server-vote-forbid-sign">forbiden</strong>';
    $row['d2d_server_voting_link'] = l(t('forbidden'), 'user/register', array('attributes' => array('class' => "d2d-server-vote-forbid-sign", 'title' => "Login or Register to Vote")));
  }
  return $row;
}

/**
 * backtoserver button link and like lint in the view page.
 */
function _d2d_server_back_like_button($did, $d2did) {
  $url = 'd2d_server_homepage';
  $backtoserver_link = l(t('Back To Server'), $url, array('attributes' => array('class' => array('backtoserver-button-link'))));
  $backtoserver_link = "<strong>$backtoserver_link</strong>";

  $voted = _d2d_server_check_whether_voted($did);
  if ($voted === 1) {
    $url = 'd2d/' . $d2did . '/undovote';
    $vote_s = l(t('undovote'), $url, array('attributes' => array('class' => array('d2d-server-undovote-link', "d2d-server-undovote-link-$did"), 'title' => "d2d-server-undovote-link-$did", 'id' => array('d2d-server-view-page-vote'))));
  }
  elseif ($voted === 0) {
    $url = 'd2d/' . $d2did . '/voting';
    $vote_s = l(t('voting'), $url, array('attributes' => array('class' => array('d2d-server-voting-link', "d2d-server-voting-link-$did"), 'title' => "d2d-server-voting-link-$did", 'id' => array('d2d-server-view-page-vote'))));
  }
  else {
    //$vote_s = '<strong class="d2d-server-vote-forbid-sign" id="d2d-server-view-page-vote" title="Login or Register to Vote">forbiden</strong>';
    $url = 'user/register';
    $vote_s = l(t('forbidden'), $url, array('attributes' => array('class' => array('d2d-server-vote-forbid-sign'), 'title' => "Login or Register to Vote", 'id' => array('d2d-server-view-page-vote'))));
  }

  return $backtoserver_link . $vote_s;
}


