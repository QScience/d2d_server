<?php
/**
 * @file
 */

/**
 * Implements ENTITY_TYPE_form for admin UI.
 */
function d2d_entity_form($form, &$form_state, $d2d_entity) {
  $form['d2d_file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a d2d file'),
    '#description' => t('Upload a file, allowed extensions: yaml, xml'),
  );


  field_attach_form('d2d_entity', $d2d_entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#weight' => 20,
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
}
