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
      '#prefix' => "No friend instances been added.<br>Go check whether there are any ",
      '#markup' => l(t('new friends'), 'admin/d2d_server/add'),
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

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Fetch Informations',
    '#weight' => 20,
  );

  return $form;
}

/**
 */
function d2d_fetchClientInfo_form_validate(&$form, &$form_state) {
}

/**
 */
function d2d_fetchClientInfo_form_submit(&$form, &$form_state) {
  //dsm($form_state);

  $ids = array();
  foreach ($form_state['values']['table'] as $key => $value) {
    _d2d_server_fetch_users_number($value);
  }
  //return "hel";
}
