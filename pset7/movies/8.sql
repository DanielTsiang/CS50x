-- In 8.sql, write a SQL query to list the names of all people who starred in Toy Story.
SELECT people.name FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE movies.title = "Toy Story";