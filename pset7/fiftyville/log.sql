-- Keep a log of any SQL queries you execute as you solve the mystery.

-- The theft took place on July 28, 2020 and it took place on Chamberlin Street.

-- Return descriptions from crime scene reports on day and street of theft
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day = 28
AND street = "Chamberlin Street";
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time,
-- each of their interview transcripts mentions the courthouse.

-- Return transcripts from interviews on day of theft
SELECT name, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28;
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
    -- If you have security footage from the courthouse parking lot,
    -- you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized.
    -- Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street
    -- and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
    -- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
    -- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Return a list of license plates that left between 10:15am - 10:25am:
SELECT license_plate, hour, minute FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28
AND hour = 10
AND minute >= 15 AND minute <=25
AND activity = "exit"
ORDER BY minute;
-- Suspect license plates:
    -- license_plate | hour | minute
    -- 5P2BI95 | 10 | 16
    -- 94KL13X | 10 | 18
    -- 6P58WS2 | 10 | 18
    -- 4328GD8 | 10 | 19
    -- G412CB7 | 10 | 20
    -- L93JTIZ | 10 | 21
    -- 322W7JE | 10 | 23
    -- 0NTHK55 | 10 | 23

-- Return a list of people details that match license plate suspects
SELECT license_plate, name, phone_number, passport_number FROM people
WHERE license_plate IN ("5P2BI95", "94KL13X", "6P58WS2", "4328GD8", "G412CB7", "L93JTIZ", "322W7JE", "0NTHK55");
-- license_plate | name | phone_number | passport_number
-- 5P2BI95 | Patrick | (725) 555-4692 | 2963008352
-- 6P58WS2 | Amber | (301) 555-4174 | 7526138472
-- L93JTIZ | Elizabeth | (829) 555-5269 | 7049073643
-- G412CB7 | Roger | (130) 555-0289 | 1695452385
-- 4328GD8 | Danielle | (389) 555-5198 | 8496433585
-- 322W7JE | Russell | (770) 555-1861 | 3592750733
-- 0NTHK55 | Evelyn | (499) 555-9472 | 8294398571
-- 94KL13X | Ernest | (367) 555-5533 | 5773159633

-- Return account numbers on day of theft at ATM location that thief was seen withdrawing money at
SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location = "Fifer Street"
AND transaction_type = "withdraw";
-- Suspect account numbers:
    -- 28500762
    -- 28296815
    -- 76054385
    -- 49610011
    -- 16153065
    -- 25506511
    -- 81061156
    -- 26013199

-- Return a list of person IDs that match the suspect account numbers
SELECT account_number, person_id from bank_accounts
WHERE account_number
IN ("28500762", "28296815", "76054385", "49610011", "16153065", "25506511", "81061156", "26013199");
-- account_number | person_id
    -- 49610011 | 686048
    -- 26013199 | 514354
    -- 16153065 | 458378
    -- 28296815 | 395717
    -- 25506511 | 396669
    -- 28500762 | 467400
    -- 76054385 | 449774
    -- 81061156 | 438727

-- Return a list of person names that match the suspect account numbers
SELECT id, name FROM people
WHERE id
IN ("686048", "514354", "458378", "395717", "396669", "467400", "449774", "438727");
-- id | name
    -- 395717 | Bobby
    -- 396669 | Elizabeth
    -- 438727 | Victoria
    -- 449774 | Madison
    -- 458378 | Roy
    -- 467400 | Danielle <-- This name matches the name of one license plate owner
    -- 514354 | Russell <-- This name matches the name of one license plate owner
    -- 686048 | Ernest <-- This name matches the name of one license plate owner

-- Return a list of phone numbers on day of theft and duration less than 1 minute
SELECT caller, receiver, duration FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28
AND duration < 60;

-- returns a list of names that made a call on day of theft and duration less than 1 minute
SELECT name, phone_number FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28
AND duration < 60);
-- Suspect phone callers:
    -- Bobby | (826) 555-1652
    -- Roger | (130) 555-0289
    -- Victoria | (338) 555-6650
    -- Madison | (286) 555-6063
    -- Russell | (770) 555-1861 <-- This name matches the name of one license plate owner and suspect account number
    -- Evelyn | (499) 555-9472
    -- Ernest | (367) 555-5533 <-- This name matches the name of one license plate owner and suspect account number
    -- Kimberly | (031) 555-6622

-- Return the receiver of suspect phone call
SELECT caller, receiver, duration FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28
AND duration < 60
AND caller IN ("(770) 555-1861", "(367) 555-5533");
-- caller | receiver | duration
-- (367) 555-5533 | (375) 555-8161 | 45
-- (770) 555-1861 | (725) 555-3243 | 49

-- Return name of receiver of suspect phone call
SELECT phone_number, name FROM people
WHERE phone_number IN ("(375) 555-8161", "(725) 555-3243");
-- phone_number | name
-- (725) 555-3243 | Philip
-- (375) 555-8161 | Berthold <-- Ernest called Berthold

-- Return passport number of suspect thiefs
SELECT name, passport_number FROM people
WHERE name IN ("Ernest", "Russell")
ORDER BY name;
-- name | passport_number
-- Ernest | 5773159633
-- Russell | 3592750733

-- Return flight IDs of suspect passport numbers
SELECT passport_number, flight_id FROM passengers
WHERE passport_number IN ("5773159633", "3592750733");
-- passport_number | flight_id
-- 3592750733 | 18
-- 3592750733 | 24
-- 5773159633 | 36 <-- Ernest
-- 3592750733 | 54

-- Return airport information of Fiftyville
SELECT * FROM airports WHERE city = "Fiftyville";
-- id | abbreviation | full_name | city
-- 8 | CSF | Fiftyville Regional Airport | Fiftyville

-- Return destination airport ID and check suspect flight IDs on 1 day after theft, earliest departure from Fiftyville
SELECT * FROM flights
WHERE year = 2020 AND month = 7 AND day = 29
AND origin_airport_id = 8
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0
-- 23 | 8 | 11 | 2020 | 7 | 29 | 12 | 15
-- 36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 <-- This is the earliest flight departing from Fiftyville on this day
-- 43 | 8 | 1 | 2020 | 7 | 29 | 9 | 30
-- 53 | 8 | 9 | 2020 | 7 | 29 | 15 | 20

-- Return airport information of airport ID 4
SELECT * FROM airports WHERE id = 4;
-- id | abbreviation | full_name | city
-- 4 | LHR | Heathrow Airport | London <-- London
