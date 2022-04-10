function openNav() {
    $('.sidebar').css('width', '450px');
    $('.main').css('margin-left', '450px');
    $('#menu-expand').attr('id', 'menu-collapse');
}
  
function closeNav() {
    $('.sidebar').css('width', '65px');
    $('.main').css('margin-left', '65px');
    $('#menu-collapse').attr('id', 'menu-expand');
}

function getTextInput() {
    const text_input = $('#text-input').val();
    return text_input.trim();
}

function clearTextInput() {
    $('#text-input').val('');
}

const socket = io();

socket.on("connect", (socket) => {
    console.log("Connected");
});

setInterval(function() {
    socket.emit('REQUEST', "UPDATE");
    
}, 900);

$( document ).ready(function() {
    $('.svg-icon').on('click', function() {
        if($(this).attr('id') === 'menu-collapse') {
            $('.side_Container').children().not('.side_title').hide();
            $('.side_title').children('a').hide();
            closeNav();
        } else {
            openNav();
            $('.side_Container').children().show();
            $('.side_title').children('a').show();
        }
    });


    $('#turnoffButton').on('click', function() {
        socket.emit('REQUEST', "QUIT");
        alert("Rover #1 off");
        $(this).hide();
    });

    $('#text-input-button').on('click', function() {
        const text = getTextInput();
        if(text !== '') {
            socket.emit('REQUEST', "TEXT:" + text);
            clearTextInput();
        }
    });

    $('#turnLeftButton').on('mousedown', function() {
        socket.emit('REQUEST', "MOTOR_LEFT");
    }).on('mouseup mouseleave', function() {
        socket.emit('REQUEST', "MOTOR_STOP")
    });

    $('#turnRightButton').on('mousedown', function() {
        socket.emit('REQUEST', "MOTOR_RIGHT");
    }).on('mouseup mouseleave', function() {
        socket.emit('REQUEST', "MOTOR_STOP")
    });

    $('#turnUpButton').on('mousedown', function() {
        socket.emit('REQUEST', "MOTOR_GO");
    }).on('mouseup mouseleave', function() {
        socket.emit('REQUEST', "MOTOR_STOP")
    });

    socket.on('commandReply', function(result) {
        const commands = result.split(",");
        console.log(commands)
        if (commands[0] == "update") {
            $("#runTime").text(commands[1]);
            $("#lightLevel").text(commands[2]);
            var str = "x:"+commands[3] + ", y:" + commands[4] + ", z:" + commands[5]
            $("#accelerometer").text(str);
            $("#temperature").text(commands[6]);
            $('#humidity').text(commands[7]);
            $('#turnoffButton').show();
        }
    });

    socket.on('canvas', function(data) {
        const canvas = $("#videostream");
        const context = canvas[0].getContext('2d');
        const image = new Image();
        image.src = "data:image/jpeg;base64,"+data;
        image.onload = function(){
            //context.scale(-1,1);
            context.height = image.height;
            context.width = image.width;
            context.drawImage(image,0,0,context.width, context.height);
            //context.drawImage(image,0,0,context.width * -1, context.height);
        }
    });
});
