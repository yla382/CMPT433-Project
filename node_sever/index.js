const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const startRouter = require('./routers/startPage.js');
const {SERVER_PORT: port = 3000} = process.env;

app.use('/', startRouter);

io.on('connection', (socket) => {
        console.log('a user connected');
      });

server.listen({ port }, () => {
        console.log(`🚀 Server ready at http://0.0.0.0:${port}`);
});