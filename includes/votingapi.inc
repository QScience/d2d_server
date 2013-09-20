<?php
/**
 * @file
 */

/**
 * implement some vote function based on votingapi framework.
 */

/**
 * used in theme/theme.inc to return a score of current did.
 */
function _d2d_server_get_voting_score($did) {
  $criteria = array(
    'entity_type' => 'd2d_entity',
    'entity_id' => $did,
    'value_type' => 'points',
    'function' => 'sum',
  );
  $vote = votingapi_select_results($criteria);
  if ($vote) {
    return $vote[0]['value'];
  }
  else {
    return 0;
  }
}

/**
 * callback hook_menu() for /d2d/did/voting
 */
function _d2d_server_voting($did) {
  $votes = array(
    'entity_type' => 'd2d_entity',
    'entity_id' => $did,
    'value_type' => 'points',
    'value' => '1',
  );
  votingapi_set_votes($votes);

  drupal_goto("d2d/$did");
}

/**
 * check whether certain user can vote or not.
 */
function _d2d_server_check_whether_voted($did) {
  global $user;
  $criteria = array(
    'entity_type' => 'd2d_entity',
    'entity_id' => $did,
    'value_type' => 'points',
    'uid' => $user->uid,
    'vote_source' => ip_address(),
  );
  $votes = votingapi_select_votes($criteria);
  return count($votes);
}
