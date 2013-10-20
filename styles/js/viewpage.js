/**
 * @file
 */
(function($){
$(document).ready(function() {
  
  $('#one_d2d_instance .fieldset-wrapper a').each(function(){
    $(this).attr("target", '_blank');
  });

  $('.field-type-taxonomy-term-reference .field-item :not(:last)').each(function() {
	  $(this).after(',');
  });

});
})(jQuery)
