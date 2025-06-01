const express = require('express');
const http = require('http');
const fs = require ('fs');
const { Server } = require('socket.io');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');


const app = express();
const server = http.createServer(app);
const io = new Server(server);
app.use(express.json());
app.use(express.static('public'));

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


app.post('/data', (req, res) => {
  const { userId, volume, timestamp } = req.body;
  console.log("Incoming data:", req.body);

  const users = JSON.parse(fs.readFileSync('users.json'));
  const user = users.find(u => u.username === userId);

  if (user) {
    user.sessions.push({ volume, timestamp });
    fs.writeFileSync('users.json', JSON.stringify(users, null, 2));
    console.log(`Added session for ${user.username}`);
    res.sendStatus(200);
  } else {
    res.status(404).send('User not found');
  }
});

app.get('/ping', (req, res) => {
  console.log("Received ping from ESP32");
  res.send("pong");
});
