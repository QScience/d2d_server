/**
 * @file
 */
(function($){
$(document).ready(function() {

  $('#d2d-server-fetch-select-all a').click(function() {
    if ($(this).text() == "Select All") {
      $('#edit-infotype input').each(function() {
        this.checked = true;
      });
      $(this).text("Unselect All");
    }
    else {
      $('#edit-infotype input').each(function() {
        this.checked = false;
      });
      $(this).text("Select All");
    }
    return false;
  });
  

});
})(jQuery)
