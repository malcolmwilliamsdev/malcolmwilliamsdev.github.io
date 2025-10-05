const pool = require('./db');

async function getAllTrips() {
    const res = await pool.query('SELECT * FROM trips');
    return res.rows;
}

async function getTripByCode(code) {
    const res = await pool.query('SELECT * FROM trips WHERE code = $1', [code]);
    return res.rows[0];
}

async function addTrip(trip) {
    const res = await pool.query(
        `INSERT INTO trips (code, name, length, start_date, resort, per_person, image, description)
         VALUES ($1, $2, $3, $4, $5, $6, $7, $8) RETURNING *`,
        [
            trip.code,
            trip.name,
            trip.length,
            trip.start_date,
            trip.resort,
            trip.per_person,
            trip.image,
            trip.description,
        ]
    );
    return res.rows[0];
}

async function updateTrip(code, trip) {
    const res = await pool.query(
        `UPDATE trips SET code=$1, name=$2, length=$3, start_date=$4, resort=$5, per_person=$6, image=$7, description=$8
         WHERE code=$9 RETURNING *`,
        [
            trip.code,
            trip.name,
            trip.length,
            trip.start_date,
            trip.resort,
            trip.per_person,
            trip.image,
            trip.description,
            code,
        ]
    );
    return res.rows[0];
}

module.exports = {
    getAllTrips,
    getTripByCode,
    addTrip,
    updateTrip,
};