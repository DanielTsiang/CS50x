// Wait for the DOM to be ready
$(function() {
	// Configure date sorting
	$.fn.dataTable.moment("DD/MM");

	// Load table
	var birthdaysDataTable = $("#birthdays").DataTable({
		scrollY: 400,
		"order": [[0, "asc"]],
		"ajax": {
			"url": "/data",
			"type": "GET",
			"dataType": "json",
			"dataSrc": "data",
			"contentType":"application/json"
		},
		"columns": [
			{"data": "name"},
			{"render": function(data, type, full, meta) {
					return full.day.toString().padStart(2,"0") + "/" + full.month.toString().padStart(2,"0");
				}
			},
			// Render 3rd column
			{"orderable": false,
			"render": function(data, type, full, meta) {
				return `
				<div style="display:block">
					${/* Button trigger edit modal */''}
					<button class="btn btn-outline-primary update-row" data-bs-toggle="modal" data-bs-target="#Modal${full.id}" data-id=${full.id}>
                        <i class="fas fa-edit"></i>
                    </button>
					${/* Edit modal */''}
					<div class="modal fade" id="Modal${full.id}" tabindex="-1" aria-labelledby="ModalLabel" aria-hidden="true">
						<div class="modal-dialog">
							<div class="modal-content">
								<div class="modal-header">
									<h5 class="modal-title" id="ModalLabel">Edit birthday</h5>
									<button class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
								</div>
								<form class="update-form" data-id=${full.id} id="update-form-${full.id}">
									<div class="modal-body">
										<input name="UpdateId" type="hidden" value=${full.id}>
										<div class="mb-3">
											<label for="UpdateName" class="form-label">Name</label>
											<input type="text" class="form-control" name="UpdateName" id="update-name-${full.id}" placeholder="Enter new name" pattern="[A-Za-z]+" autocomplete="off" autofocus>
										</div>
										<div class="mb-3">
											<label for="UpdateDay" class="form-label">Day</label>
											<input type="number" class="form-control" name="UpdateDay" id="update-day-${full.id}" placeholder="Enter new day" min="1" max="31" step="1" autocomplete="off">
										</div>
										<div class="mb-3">
											<label for="UpdateMonth" class="form-label">Month</label>
											<input type="number" class="form-control" name="UpdateMonth" id="update-month-${full.id}" placeholder="Enter new month" min="1" max="12" step="1" autocomplete="off">
										</div>
									</div>
									<div class="modal-footer">
										<button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
										<button type="submit" class="btn btn-primary">Save changes</button>
									</div>
								</form>
							</div>
						</div>
					</div>
					${/* Button to delete row */''}
					<form>
						<button type="button" class="btn btn-outline-danger delete-row" data-id=${full.id}>
							<i class="fas fa-trash-alt"></i>
						</button>
					</form>
				</div>`
				}
			},
		]
	});

	// Delete row
	$(document).on("click", ".delete-row", function(event){
		let delete_id = $(this).data("id");

		// AJAX request
		$.ajax({
			url: "/delete",
			type: "POST",
			data: {id: delete_id},
			success: function(response){

				// Reload DataTable
				birthdaysDataTable.ajax.reload();

				// Show delete birthday alert, and auto slideUp
				$("#delete-birthday").fadeIn().fadeTo(2000, 500).slideUp(500, function() {
			    	$("#delete-birthday").slideUp(500);
			    });
			}
		});
	});

	// Validator default settings for forms
	$.validator.setDefaults({
		errorElement: "span",
		errorClass: "has-error",
		validClass: "has-success",
		highlight: function(element, errorClass, validClass) {
			$(element).closest('.form-control').addClass(errorClass).removeClass(validClass);
		},
		unhighlight: function(element, errorClass, validClass) {
			$(element).closest('.form-control').removeClass(errorClass).addClass(validClass);
		},
		errorPlacement: function(error, element) {
			error.insertAfter(element);
		},
		success: function(label) {
			label.addClass("has-success").text("Ok!")
		},
	});

	// Insert row, with jQuery form validation
	$("#birthday-form").validate({
		rules: {
			name: {
				required: true,
				lettersonly: true
			},
			day: {
				required: true,
				integer: true,
				min: 1,
				max: 31
			},
			month: {
				required: true,
				integer: true,
				min: 1,
				max: 12
			},
		},
		messages: {
			name: {
				required: "Please enter a name"
			},
			day: {
				required: "Please enter a day"
			},
			month: {
				required: "Please enter a month"
			}
		},
		submitHandler: function(form) {
			$.ajax({
				url: "/",
				method: "POST",
				data: {
					name: $(form).find("input[name='name']").val(),
					day: $(form).find("input[name='day']").val(),
					month: $(form).find("input[name='month']").val(),
				},
				dataType: "json",
				success: function() {
					// Remove validation classes and errorElements
					$("input, span").removeClass("has-error has-success");
					$("span").remove()

					// Reload DataTable
					birthdaysDataTable.ajax.reload();

					// Show add birthday alert and auto slideUp
					$("#add-birthday").fadeIn().fadeTo(2000, 500).slideUp(500, function() {
				    	$("#add-birthday").slideUp(500);
				    });
				},
				error: function(xhr, status, error) {
				    alert(`${status}: ${error}`);
				}
			});
			$("#birthday-form")[0].reset();
		}
	});

	// Update row
	$(document).on("submit", ".update-form", function(event){
		let update_id = $(this).data("id");

		// AJAX request
		$.ajax({
			url: "/update",
			method: "POST",
			data: {
				UpdateId: $(this).find("input[name='UpdateId']").val(),
				UpdateName: $(this).find("input[name='UpdateName']").val(),
				UpdateDay: $(this).find("input[name='UpdateDay']").val(),
				UpdateMonth: $(this).find("input[name='UpdateMonth']").val(),
			},
			dataType: "json",
			success: function(update_id) {
				// Hide and remove Modal
				$(`#Modal${update_id}`).modal('hide');
				$('body').removeClass('modal-open');
				$('.modal-backdrop').remove();

				// Reload DataTable
				birthdaysDataTable.ajax.reload();

				// Show update birthday alert, and auto slideUp
				$("#update-birthday").fadeIn().fadeTo(2000, 500).slideUp(500, function() {
			    	$("#update-birthday").slideUp(500);
			    });
			},
			error: function(xhr, status, error) {
			    alert(`${status}: ${error}`);
			}
		});
		// Prevent page refresh
		event.preventDefault();
	});

	// Fade out dismissible alert message, allows alert message to fade back in later.
	$(function(){
		$("[data-hide]").on("click", function() {
			$(this).closest("." + $(this).attr("data-hide")).fadeOut();
		});
	});

	// Update row, with jQuery form validation
	// $(document).on("click", ".update-row", function(event){
	// 	let update_id = $(this).data("id");

	// 	$(`#update-form-${update_id}`).validate({
	// 		submitHandler: function(form) {
	// 			// AJAX request
	// 			$.ajax({
	// 				url: "/update",
	// 				method: "POST",
	// 				data: {
	// 					UpdateId: update_id,
	// 					UpdateName: $(`input[id='update-name-${update_id}']`).val(),
	// 					UpdateDay: $(`input[id='update-day-${update_id}']`).val(),
	// 					UpdateMonth: $(`input[id='update-month-${update_id}']`).val(),
	// 				},
	// 				dataType: "json",
	// 				success: function(update_id) {
	// 					// Hide and remove Modal
	// 					$(`#Modal${update_id}`).modal('hide');
	// 					$('body').removeClass('modal-open');
	// 					$('.modal-backdrop').remove();

	// 					// Reload DataTable
	// 					birthdaysDataTable.ajax.reload();

	// 					// Show update birthday alert, and auto slideUp
	// 					$("#update-birthday").fadeIn().fadeTo(2000, 500).slideUp(500, function() {
	// 				    	$("#update-birthday").slideUp(500);
	// 				    });
	// 				},
	// 				error: function(xhr, status, error) {
	// 				    alert(`${status}: ${error}`);
	// 				}
	// 			});
	// 			// Prevent page refresh
	// 			event.preventDefault();
	// 		}
	// 	});
	// });

});
