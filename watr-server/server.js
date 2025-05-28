const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');


const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static('public'));

const arduinoPort = 'COM7'; 

const port = new SerialPort({
  path: 'COM7',
  baudRate: 115200
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));


parser.on('data', data => {
  console.log('Arduino data:', data.trim());
  io.emit('arduino-data', data.trim());
});

port.on('error', err => {
  console.error('Serial port error:', err.message);
});

io.on('connection', (socket) => {
  console.log('Client connected');
  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
