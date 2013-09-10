<?php
/**
 * @file
 */

/**
 * customise entity controller class.
 */
class D2DServerEntityController extends EntityAPIController {

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    $entity->created = REQUEST_TIME;

    return parent::save($entity, $transaction);
  }

   /**
   * Implements EntityAPIControllerInterface.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    // For Field API and entity_prepare_view, the entities have to be keyed by
    // (numeric) id.
    $entities = entity_key_array_by_property($entities, $this->idKey);
    if (!empty($this->entityInfo['fieldable'])) {
      field_attach_prepare_view($this->entityType, $entities, $view_mode);
    }
    entity_prepare_view($this->entityType, $entities);
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

    $view = array();
    foreach ($entities as $entity) {
      $build = entity_build_content($this->entityType, $entity, $view_mode, $langcode);


      foreach ($build as $key => $value) {
        if (isset($value['#field_type']) and $value['#field_type'] == 'taxonomy_term_reference') {
          if (isset($value['#items'])) {
            foreach ($value['#items'] as $_key => $_value) {
              $tid = $_value['tid'];
              $tags_name = $value['#field_name'];
              $build[$key][$_key]['#href'] = "d2d/tags/$tags_name/$tid";
            }
          }
        }
      }

      $build += array(
        // If the entity type provides an implementation, use this instead the
        // generic one.
        // @see template_preprocess_entity()
        '#theme' => 'entity',
        '#entity_type' => $this->entityType,
        '#entity' => $entity,
        '#view_mode' => $view_mode,
        '#language' => $langcode,
        '#page' => $page,
      );
      // Allow modules to modify the structured entity.
      drupal_alter(array($this->entityType . '_view', 'entity_view'), $build, $this->entityType);
      $key = isset($entity->{$this->idKey}) ? $entity->{$this->idKey} : NULL;
      $view[$this->entityType][$key] = $build;
    }
    return $view;
  }



}
