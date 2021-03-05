-- In 6.sql, write a SQL query to determine the average rating of all movies released in 2012.
-- SELECT AVG(rating) FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);
SELECT AVG(ratings.rating) FROM movies
JOIN ratings
ON movies.id = ratings.movie_id
WHERE movies.year = 2012;