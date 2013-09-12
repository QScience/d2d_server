<?php
/**
 * @file
 */

/**
 * Implements entity class.
 */
class D2DEntity extends Entity {

  /**
   * Override this in order to implement a custom default URI and specify
   * 'entity_class_uri' as 'uri callback' hook_entity_info().
   */
  protected function defaultUri() {
    return array('path' => 'd2d/' . $this->identifier());
  }


}
