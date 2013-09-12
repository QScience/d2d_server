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
  $options = array();
  foreach ($newfriends as $key => $value) {
    $options[$key] = $key;
  }

  $form['check'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('import what?'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  //field_attach_form('d2d_entity', $d2d_entity, $form, $form_state);

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
  $op = $form_state['values']['check'];
  //dsm($op);
  foreach ($op as $key => $value) {
    //dsm($value);
    if ($value === 0) {
      continue;
    }
    d2d_server_get_instance_byd2did($key);
  }
}
