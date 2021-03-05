-- In 9.sql, write a SQL query to list the names of all people who starred in a movie released in 2004, ordered by birth year.
-- If a person appeared in more than one movie in 2004, they should only appear in your results once.
SELECT DISTINCT(people.name) FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE movies.year = 2004 ORDER BY people.birth;