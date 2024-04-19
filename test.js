const express = require('express');
const app = express();

app.get('/', (req, res) => {
  const userComment = req.query.comment || '';
  res.send(`<html><body><h1>User comment: ${userComment}</h1></body></html>`);
});

app.listen(3000, () => {
  console.log('Server listening on port 3000');
});