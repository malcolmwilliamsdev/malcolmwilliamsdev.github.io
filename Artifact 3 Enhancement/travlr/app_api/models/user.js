const pool = require('./db');
const crypto = require("crypto");
const jwt = require("jsonwebtoken");

async function findUserByEmail(email) {
  const res = await pool.query('SELECT * FROM users WHERE email = $1', [email]);
  return res.rows[0];
}

async function createUser({ name, email, password }) {
  const salt = crypto.randomBytes(16).toString("hex");
  const hash = crypto.pbkdf2Sync(password, salt, 1000, 64, "sha512").toString("hex");
  const res = await pool.query(
    'INSERT INTO users (name, email, salt, hash) VALUES ($1, $2, $3, $4) RETURNING *',
    [name, email, salt, hash]
  );
  return res.rows[0];
}

function validatePassword(user, password) {
  const hash = crypto.pbkdf2Sync(password, user.salt, 1000, 64, "sha512").toString("hex");
  return user.hash === hash;
}

function generateJWT(user) {
  return jwt.sign(
    {
      id: user.id,
      email: user.email,
      name: user.name,
    },
    process.env.JWT_SECRET,
    { expiresIn: "1h" }
  );
}

module.exports = {
  findUserByEmail,
  createUser,
  validatePassword,
  generateJWT,
};
