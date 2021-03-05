(function($) {
	"use strict"; // Start of use strict

	// Smooth scrolling using jQuery easing
	$('a.js-scroll-trigger[href*="#"]:not([href="#"])').click(function() {
		if (location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '') && location.hostname == this.hostname) {
			var target = $(this.hash);
			target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
			if (target.length) {
				$('html, body').animate({
					scrollTop: (target.offset().top)
				}, 1000, "easeInOutExpo");
				return false;
			}
		}
	});

	// Closes responsive menu when a scroll trigger link is clicked
	$('.js-scroll-trigger').click(function() {
		$('.navbar-collapse').collapse('hide');
	});

	// Activate scrollspy to add active class to navbar items on scroll
	$('body').scrollspy({
		target: '#sideNav'
	});

	// Validates the contact form and submits it using Formspree.
	$.validator.setDefaults({
		highlight: function(element) {
			$(element).closest('.form-group').addClass('has-error');
		},
		unhighlight: function(element) {
			$(element).closest('.form-group').removeClass('has-error');
		},
		errorElement: 'span',
		errorClass: 'help-block',
		errorPlacement: function(error, element) {
			if(element.parent('.input-group').length) {
				error.insertAfter(element.parent());
			} else {
				error.insertAfter(element);
			}
		}
	});

	$("#contact-form").validate({
		submitHandler: function(form) {
			$.ajax({
				url: "//formspree.io/f/mnqoklgo",
				method: "POST",
				data: {
					name: $(form).find("input[name='name']").val(),
					_replyto: $(form).find("input[name='_replyto']").val(),
					_subject: $(form).find("input[name='_subject']").val(),
					message: $(form).find("textarea[name='message']").val()
				},
				dataType: "json",
				success: function() {
					$("#submit-success").fadeIn();
					$("#contact-form").fadeOut();
				},
				error: function() {
					$("#submit-errors").fadeIn();
				}
			});
		}
	});

	// Hides alert message for form submission error,
	// allows alert message to reappear if user tries to resubmit form.
	$(function(){
		$("[data-hide]").on("click", function() {
			$(this).closest("." + $(this).attr("data-hide")).hide();
		});
	});

})(jQuery); // End of use strict
