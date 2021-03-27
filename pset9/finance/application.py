import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, datetimeformat

# Configure application
app = Flask(__name__)


# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filters
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["datetimeformat"] = datetimeformat


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db", connect_args={"check_same_thread": False})

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Access user's id
    user_id = session["user_id"]

    # Select information from shares table for logged in user
    SHARES = db.execute("SELECT * FROM shares WHERE user_id = ?", user_id)

    # Initialise OVERALL_TOTAL variable
    OVERALL_TOTAL = 0

    # Obtain latest share's price from API and update database
    for share in SHARES:
        QUOTED = lookup(share["symbol"])
        new_shares_total = share["shares_count"] * QUOTED["price"]
        OVERALL_TOTAL += new_shares_total
        db.execute("UPDATE shares SET price = ? AND total = ? WHERE user_id = ? AND symbol = ?",
                   QUOTED["price"], new_shares_total, user_id, share["symbol"])
        share["price"] = QUOTED["price"]
        share["total"] = new_shares_total

    # Check how much cash the user currently has
    CASH = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    # Calculate final value for OVERALL_TOTAL
    OVERALL_TOTAL += CASH

    return render_template("index.html", shares=SHARES, cash=CASH, overall_total=OVERALL_TOTAL)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Access form data
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Access user's id
        user_id = session["user_id"]

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Ensure shares was submitted
        if not shares:
            return apology("must provide shares", 400)

        # Check if submitted shares string is a positive integer
        if not shares.isdigit() :
            return apology("shares is not a number", 400)
        # Shares is valid
        else:
            shares = int(shares)

        # Obtain quote using lookup function
        QUOTED = lookup(symbol)

        # Ensure valid symbol was submitted
        if QUOTED is None:
            return apology("invalid symbol", 400)

        # Check if user has enough cash to buy shares
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        cost = QUOTED["price"] * shares
        if cash < cost:
            return apology("can't afford", 400)

        # New amount of cash user has after buying shares
        new_cash_total = cash - cost

        # Update cash in users table for user
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash_total, user_id)

        # Insert buy log into history table
        db.execute("INSERT INTO history (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, datetime('now'))",
                   user_id, QUOTED["symbol"], shares, QUOTED["price"])

        # Keep track of shares in shares table
        current_shares = db.execute("SELECT shares_count FROM shares WHERE user_id = ? AND symbol = ?", user_id, QUOTED["symbol"])

        # If shares have not been bought before
        if not current_shares:
            db.execute("INSERT INTO shares VALUES (?, ?, ?, ?, ?, ?)",
                       user_id, QUOTED["symbol"], QUOTED["name"], shares, QUOTED["price"], QUOTED["price"])

        # If shares have been bought before
        else:
            new_shares_total = current_shares[0]["shares_count"] + shares
            shares_value_total = new_shares_total * QUOTED["price"]
            db.execute("UPDATE shares SET shares_count = ?, price = ?, total = ? WHERE user_id = ? AND symbol = ?",
                       new_shares_total, QUOTED["price"], shares_value_total, user_id, QUOTED["symbol"])

        # Redirect user to home page
        flash("Bought!", "info")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Access user's id
    user_id = session["user_id"]

    # Obtain history information for logged in user
    TRANSACTIONS = db.execute("SELECT * FROM history WHERE user_id = ? ORDER BY transacted DESC", user_id)

    return render_template("history.html", transactions=TRANSACTIONS)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Access form data
        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Access form data
        quote = request.form.get("symbol")

        # Ensure quote was submitted
        if not quote:
            return apology("must provide quote", 400)

        # Obtain quote using lookup function
        QUOTED = lookup(quote)

        # Invalid symbol provided
        if not QUOTED:
            return apology("invalid symbol", 400)

        # Render quoted template
        return render_template("quoted.html", quoted=QUOTED)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Access form data
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure password matches password confirmation
        elif password != confirmation:
            return apology("passwords do not match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Username does not already exist
        if not rows:

            # Insert data into database
            user_id = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

            # Remember which user has logged in
            session["user_id"] = user_id

            # Redirect user to home page
            return redirect("/")

        # Username already exists
        else:
            flash("Username is already taken", "danger")
            return apology("Username is already taken", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Access user's id
    user_id = session["user_id"]

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Access form data
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Ensure shares was submitted
        if not shares:
            return apology("must provide shares", 400)

        # Obtain quote using lookup function
        QUOTED = lookup(symbol)

        # Check if user has enough shares to sell as requested
        shares_count = db.execute("SELECT shares_count FROM shares WHERE user_id = ? AND symbol = ?",
                                  user_id, QUOTED["symbol"])[0]["shares_count"]
        if shares > shares_count:
            return apology("not enough shares owned", 400)

        # User has enough shares to sell as requested
        else:
            # Calculate new cash amount user has
            cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
            cash_gained = QUOTED["price"] * shares
            new_cash_total = cash + cash_gained

            # Update cash in users table for user
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash_total, user_id)

            # Insert sell log into history table
            db.execute("INSERT INTO history (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, datetime('now'))",
                       user_id, QUOTED["symbol"], -(shares), QUOTED["price"])

            # Keep track of shares in shares table
            current_shares = db.execute("SELECT shares_count FROM shares WHERE user_id = ? AND symbol = ?",
                                        user_id, QUOTED["symbol"])[0]["shares_count"]
            new_shares_total = current_shares - shares

            # If 0 shares left of the stock owned
            if new_shares_total == 0:
                db.execute("DELETE FROM shares WHERE user_id = ? AND symbol = ?", user_id, QUOTED["symbol"])

                # Redirect user to home page
                flash("Sold!", "info")
                return redirect("/")

            # User still owns shares of the stock
            else:
                shares_value_total = new_shares_total * QUOTED["price"]
                db.execute("UPDATE shares SET shares_count = ?, price = ?, total = ? WHERE user_id = ? AND symbol = ?",
                           new_shares_total, QUOTED["price"], shares_value_total, user_id, QUOTED["symbol"])

                # Redirect user to home page
                flash("Sold!", "info")
                return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Select share symbols from shares table for logged in user
        SHARES = db.execute("SELECT symbol FROM shares WHERE user_id = ?", user_id)

        return render_template("sell.html", shares=SHARES)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)