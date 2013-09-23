<?php
/**
 * @file
 */

define("D2DSERVER_NOSEARCHRESULT", " <div class='patterns-server-list-table-wrap'>nothing found.</div>");
module_load_include('inc', 'd2d_server', 'includes/tags');

/**
 * Implement search function form for patterns_server.
 */
function d2d_server_search($form, &$form_state, $search_type='', $search_value='') {
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'd2d_server') . '/styles/js/search.js',
  );
  $form['d2d_server_search']= array(
    '#type' => 'container',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['d2d_server_search']['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#title_display' => 'invisible',
    '#size' => 15,
    '#default_value' => ($search_value == '')?'':$search_value,
    '#attributes' => array('title' => t('Enter the terms you wish to search for.'), 'class' => array('container-inline')),

  );
  $form['d2d_server_search']['selected'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#title_display' => 'invisible',
    '#options' => array(
      0 => t('Title'),
      1 => t('Category'),
      2 => t('D2DID'),
      3 => t('Tags'),
    ),
    '#default_value' => ($search_type == '')?0:$search_type,
  );
  $button_url = url('d2d/search/');
  $form['d2d_server_search']['actions'] = array('#type' => 'actions');
  $form['d2d_server_search']['actions']['submit'] = array('#type' => 'submit', '#value' => t('Search'), '#attributes' => array('name' => $button_url));
  $form['d2d_server_search']['#submit'][] = 'd2d_server_search_submit';

  return $form;
}

/**
 * search form's submit function.
 */
function d2d_server_search_submit($form, &$form_state) {

  if ($form_state['values']['search'] == '') {
    form_set_error('keys', t('Please enter some keywords.'));
  }

  $form_id = $form['form_id']['#value'];
  $form_state['redirect'] = 'd2d/search/' . trim($form_state['values']['selected']) . '/' . trim($form_state['values']['search']);

}

/**
 * callback hook_menu() for /d2d/search/arg1/arg2 path
 * search type: title, category, d2did, tags. four types.
 * every type are implemented as an option.
 * this is a core function.
 *    title is good enough with db_like.
 *
 */
function _d2d_server_search($selected, $value) {
  drupal_set_title(t('search :'));
  //drupal_add_js(drupal_get_path('module', 'd2d_server') . '/styles/js/search.js');
  $form = drupal_get_form('d2d_server_search', $selected, $value);
  $list = drupal_render($form);

  $query;
  $fieldset_title;

  switch ($selected) {
    case 0:
      drupal_set_title(t('d2d instances with title: ' . $value));
      $fieldset_title = "d2d instances with title: $value";
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'd2d_entity')
        ->propertyCondition('title', '%' . db_like($value) . '%', 'LIKE')
        ->propertyOrderBy('created', 'DESC')
        ->pager(10)
        ->execute();
      $list .= '<div id="d2d-server-all-table-wrap">';
      if (isset($query->ordered_results) && count($query->ordered_results)) {
        $list .= _d2d_server_list_page($query, $fieldset_title);
      }
      else {
        $list .= D2DSERVER_NOSEARCHRESULT;
      }
      $list .= '</div>';
      return $list;
      break;

    case 1:
      drupal_set_title(t('d2d instances with category: ' . $value));
      $split_strings = preg_split('/[\,]+/', $value);
      $sign_has_table = FALSE;
      $list .= '<div id="d2d-server-all-table-wrap">';
      foreach ($split_strings as $category_name) {
        $a_table = _d2d_server_list('category', trim($category_name));
        if ($a_table) {
          $sign_has_table = TRUE;
          $list .= $a_table;
        }
      }
      if (!$sign_has_table) {
        $list .= D2DSERVER_NOSEARCHRESULT;
      }
      $list .= '</div>';
      return $list;
      break;

    case 2:
      drupal_set_title(t('d2d instances with: ' . $value));
      $fieldset_title = "d2d instances with d2did: $value";
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'd2d_entity')
        ->propertyCondition('d2did', $value)
        ->propertyOrderBy('created', 'DESC')
        ->pager(10)
        ->execute();
      $list .= '<div id="d2d-server-all-table-wrap">';
      if (isset($query->ordered_results) && count($query->ordered_results)) {
        $list .= _d2d_server_list_page($query, $fieldset_title);
      }
      else {
        $list .= D2DSERVER_NOSEARCHRESULT;
      }
      $list .= '</div>';
      return $list;
      break;

    case 3:
      drupal_set_title(t('d2d instances with tags: ' . $value));
      $tagtids = _d2d_server_findtagtid_by_tagname($value);
      $tags_table_db = _d2d_server_get_all_taxonomyfields_name('d2d_entity');
      $list .= '<div id="d2d-server-all-table-wrap">';
      if (count($tags_table_db) && count($tagtids)) {
        foreach ($tags_table_db as $field_name) {
          foreach ($tagtids as $tid => $_temp) {
            $list .= _d2d_server_list($field_name, $tid);
          }
        }
      }
      else {
        $list .= D2DSERVER_NOSEARCHRESULT;
      }
      $list .= '</div>';
      return $list;
      break;

    default:
      drupal_set_message(t("The d2d instances list doesn't exist."), "error");
      drupal_goto('d2d_server_homepage');
  }
}
