function question_1() {
    // jQuery event for when correct answer is clicked
    $('.correct').click(function(event) {
        // reset color of other buttons
        $('button').removeClass('green red')

        // set correct button and feedback color to green
        $(event.target).addClass('green');
        $('#feedback1').css("color", "green");
        $('#feedback1').html(`${event.target.innerHTML} is the correct answer!`);
    });

    // jQuery event for when incorrect answer is clicked
    $('.incorrect').click(function(event) {
        // reset color of other buttons
        $('button').removeClass('green red')

        // set incorrect button and feedback color to red
        $(event.target).addClass('red');
        $('#feedback1').css("color", "red");
        $('#feedback1').html(`${event.target.innerHTML} is incorrect.`);
    });
};

function question_2() {
    // jQuery event for when check answer button is clicked
    $('#q2_button').click(function(event) {
        // check if correct answer submitted in textbox
        check_q2_answer();
    });

    if ($('#q2_input').on('keyup keypress', function(event) {
        // reset background color to transparent and remove feedback if user deletes text in textbox
        if (event.key === 'Backspace' || event.key === 'Delete') {
            $('#q2_input').css("background", "transparent");
            $('#feedback2').html('');
        // check question 2 answer if user hits Enter key
        } else if (event.key === 'Enter') {
            check_q2_answer();
            // prevent Enter key from submitting form
            event.preventDefault();
            return false;
        }
    }));
};

function check_q2_answer() {
    if ($('#q2_input').val().toLowerCase() == 'taiwan') {
        $('#q2_input').css("background-color", "LightGreen");
        $('#feedback2').css("color", "green");
        $('#feedback2').html('Taiwan is the correct answer!');
    } else if ($('#q2_input').val() == '') {
        $('#feedback2').css("color", "black");
        $('#feedback2').html('Please type a response into the textbox.');
    } else {
        $('#q2_input').css("background-color", "LightCoral");
        $('#feedback2').css("color", "red");
        $('#feedback2').html(`${q2_input.value} is incorrect. The correct answer was Taiwan!`);
    }
};

question_1();
question_2();