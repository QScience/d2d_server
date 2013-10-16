<?php
/**
 * @file
 */

/**
 */
function d2d_fetchClientInfo_form($form, &$form_state) {

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'd2d_entity');
  $query->propertyOrderBy('created', 'DESC')
    ->pager(10)
    ->execute();

  $entities;
  if (isset($query->ordered_results) && count($query->ordered_results)) {
    $ids = array();
    foreach ($query->ordered_results as $value) {
      $ids[] = $value->entity_id;
    }
    $entities = entity_load('d2d_entity', $ids);
  }
  else {
    $form['noavaild2d'] = array(
      '#prefix' => "No friend instances been added.<br>Check if you can already import ",
      '#markup' => l(t('some friends'), 'admin/d2d_server/add'),
      '#suffix' => '.',
    );
    return $form;
  }

  $header = array(
    'd2did' => t('D2D ID'),
    'title' => t('TITLE'),
    'description' => t('DESCRIPTION'),
    'infofetched' => t('LAST FETCH TIME'),
  );

  $options = array();
  $default_v = array();
  $i = 1;
  foreach ($entities as $entity) {
    $fetchtime = date('j-m-Y H:i:s (P)', $entity->infofetched);
    $options[$entity->d2did] = array(
      'd2did' => $entity->d2did,
      'title' => $entity->title,
      'description' => $entity->description,
      'infofetched' => $fetchtime,
    );
    if ($i == 1) {
      $default_v[$entity->d2did] = $entity->d2did;
      $i++;
    }
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    //'#default_value' => $default_v,
    //'#ajax' => array(
    //  'callback' => 'd2d_entity_form_ajax_callback',
    //  'wrapper' => 'd2d-entity-form-d2d-instance-fieldset',
    //  'method' => 'replace',
    //  'effect' => 'fade',
    //),
  );

  $options = array(
    1 => 'Users Number',
    2 => 'Nodes Number',
    3 => 'Site Name&Slogan',
    4 => 'Tags',
    5 => 'D2d Instance Description',
    6 => 'D2d Instance Friendship',
  );
  $form['infotype'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t("Information to fetch:"),
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['selectall'] = array(
    '#prefix' => '<div id ="d2d-server-fetch-select-all">',
    '#markup' => l(t('Select All'), 'admin/d2d_server/fetchClientInfo'),
    '#suffix' => '</div>',
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['fetchselected'] = array(
    '#type' => 'submit',
    '#value' => 'Fetch Data',
    '#submit' => array('d2d_fetchClientInfo_form_selected_submit'),
    '#weight' => 20,
  );
  //$form['actions']['fetchall'] = array(
  //  '#type' => 'submit',
  //  '#value' => 'Fetch All Kinds of Informations',
  //  '#submit' => array('d2d_fetchClientInfo_form_all_submit'),
  //  '#weight' => 21,
  //);
  $form['#attached']['css'][] = drupal_get_path('module', 'd2d_server') . '/styles/css/fetch_form.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'd2d_server') . '/styles/js/fetch_form.js';
  return $form;
}

/**
 */
function d2d_fetchClientInfo_form_validate(&$form, &$form_state) {
}

/**
 */
function d2d_fetchClientInfo_form_selected_submit(&$form, &$form_state) {

  $d2dids = array();
  $methods = array();

  foreach ($form_state['values']['table'] as $key => $value) {
    if ($value === 0) {
      continue;
    }
    $d2dids[] = $value;
  }
  if (count($d2dids) === 0) {
    drupal_set_message(t("Please choose at least one d2d instance."), 'error');
    return;
  }

  foreach ($form_state['values']['infotype'] as $key => $value) {
    if ("$key" === $value) {
      switch ($key) {
      case 1:
        $methods['users_number'] = TRUE;
        break;
      case 2:
        $methods['nodes_number'] = TRUE;
        break;
      case 3:
        $methods['site_info'] = TRUE;
        break;
      case 4:
        $methods['tags'] = TRUE;
        break;
      case 5:
        $methods['d2d_description'] = TRUE;
        break;
      case 6:
        $methods['friendship'] = TRUE;
        break;
      default:
      }
    }
  }
  if (count($methods) === 0) {
    drupal_set_message(t("Please select the type of information to fetch."), 'error');
    return;
  }

  foreach ($d2dids as $d2did) {
    $return = _d2d_server_fetch_infotypes($d2did, $methods);
    if ($return === 0) {
      drupal_set_message(t('%d2did Fetch Informations Successfully.', array('%d2did' => $d2did)));
    }
  }
}

/**
 */
function d2d_fetchClientInfo_form_all_submit(&$form, &$form_state) {

  $d2dids = array();
  foreach ($form_state['values']['table'] as $key => $value) {
    if ($value === 0) {
      continue;
    }
    $d2dids[] = $value;
  }
  if (count($d2dids) === 0) {
    drupal_set_message(t("Please choose at least one d2d instance."), 'error');
    return;
  }

//  $methods = array();
//  $methods['users_number'] = TRUE;
//  $methods['nodes_number'] = TRUE;
//  $methods['site_info'] = TRUE;
//  $methods['tags'] = TRUE;
//  $methods['friendship'] = TRUE;

  foreach ($d2dids as $d2did) {
    $return = _d2d_server_fetch_infotypes($d2did);
    if ($return === 0) {
      drupal_set_message(t('%d2did Fetch Informations Successfully.', array('%d2did' => $d2did)));
    }
  }
}
