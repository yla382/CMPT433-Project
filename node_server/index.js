const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const startRouter = require('./routers/startPage.js');
const {SERVER_PORT: port = 3000} = process.env;
const dgram = require('dgram');
const config = require('./config.js');
const cors = require('cors');
const child = require('child_process');

app.use(cors());

app.use('/', startRouter);

io.on('connection', (socket) => {
        console.log('a user connected');

        socket.on('REQUEST', (data) => {
                var client = dgram.createSocket('udp4');
                var buffer = new Buffer(data);
                client.send(buffer, 0, buffer.length, config.rover_1_port, config.rover_1_ip, function(err, bytes) {
                        if (err)  {
                                throw err;
                        }
                }) ;


                client.on('listening', function () {
                        var address = client.address();
                });

                // Handle an incoming message over the UDP from the local application.
                client.on('message', function (message, remote) {
                        var reply = message.toString('utf8');
                        console.log(reply);
                        socket.emit('commandReply', reply);

                        client.close();

                });
        });

        let ffmpeg = child.spawn("ffmpeg", [
                "-re", 
                "-y", 
                "-i", 
                "udp://192.168.7.1:1234",
                "-preset",  
                "ultrafast", 
                "-f", 
                "mjpeg", 
                "pipe:1"
                ]);
        //let ffmpeg = child.spawn("./video");
        ffmpeg.on('error', function (err) {
                console.log(err);
                throw err;
        });

        ffmpeg.on('close', function (code) {
                console.log('ffmpeg exited with code ' + code);
        });

        ffmpeg.stderr.on('data', function(data) {
                // Don't remove this
                // Child Process hangs when stderr exceed certain memory
        });

        ffmpeg.stdout.on('data', function (data) {
                var frame = Buffer.from(data).toString('base64');
                io.sockets.emit('canvas',frame);
        });
});

server.listen({ port }, () => {
        console.log(`🚀 Server ready at http://0.0.0.0:${port}`);
});