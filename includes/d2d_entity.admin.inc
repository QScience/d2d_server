<?php
/**
 * @file
 */
module_load_include('inc', 'd2d_server', 'includes/db');

/**
 * Implements ENTITY_TYPE_form for admin UI.
 */
function d2d_entity_form($form, &$form_state, $d2d_entity) {
  $newfriends = d2d_server_get_instances_newfriends();

  $header = array(
    'd2did' => t('D2D ID'),
    'title' => t('TITLE'),
    'description' => t('DESCRIPTION'),
    'url' => t('URL'),
  );

  $options = array();
  foreach ($newfriends as $key => $value) {
    $instance = d2d_server_get_instance_by_d2did($key);
    $options[$key] = array(
      'd2did' => $instance->d2d_id,
      'title' => $instance->name,
      'description' => $instance->description,
      'url' => $instance->url,
    );
  }
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * validate function for ENTITY_TYPE_form
 */
function d2d_entity_form_validate(&$form, &$form_state) {
}

/**
 * submit function for ENTITY_TYPE_form.
 */
function d2d_entity_form_submit(&$form, &$form_state) {
  $op = $form_state['values']['table'];
  $op = array_filter($op);
  foreach ($op as $key => $value) {
    d2d_server_import_instance_byd2did($key);
  }
}
