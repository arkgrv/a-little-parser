/// Describes the type of a single Token
///
/// Options:
/// * `OpenParen`: open parentheses
/// * `CloseParen`: close parentheses
/// * `Number`: number
/// * `OpPlus`: addition operation
/// * `OpMinus`: subtraction operation
/// * `OpStar`: multiplication operation
/// * `OpDiv`: division operation
/// * `OpExp`: power operation
/// * `IgnSpace`: ignored space 
/// * `Tbd`: to be defined
/// * `Empty`: empty token
#[derive(Copy, Clone, PartialEq, Eq)]
pub enum TokenType {
    OpenParen,
    CloseParen,
    Number,
    OpPlus,
    OpMinus,
    OpStar,
    OpDiv,
    OpExp,
    IgnSpace,
    Tbd,
    Empty,
}

/// Describes a single token
#[derive(Clone)]
pub struct Token {
    pub t_type: TokenType,
    pub value: String,
}

impl Token {
    /// Constructs a new Token
    ///
    /// Parameters:
    /// * `t_type`: type of the token
    /// * `value`: value of the token as string
    pub fn new(t_type: TokenType, value: &str) -> Token {
        Token {
            t_type,
            value: value.to_string(),
        }
    }

    /// Constructs a new Token with value
    ///
    /// Parameters:
    /// * `value`: value of the token
    pub fn new_with_value(value: &str) -> Token {
        Token {
            t_type: TokenType::Tbd,
            value: value.to_string(),
        }
    }

    /// Constructs a new Token with given type
    ///
    /// Parameters
    /// * `t_type`: type of the new token
    pub fn new_with_type(t_type: TokenType) -> Token {
        Token {
            t_type,
            value: "".to_string(),
        }
    }

    /// Constructs a new empty Token
    pub fn new_empty() -> Token {
        Token {
            t_type: TokenType::Empty,
            value: "".to_string(),
        }
    }

    /// Extracts and returns the next token
    ///
    /// Parameters:
    /// * `input`: input string to tokenize
    pub fn next_token(input: &mut String) -> Option<Token> {
        if input.is_empty() {
            return None;
        }

        match input.chars().next().unwrap() {
            '+' => {
                input.remove(0);
                return Some(Token::new(TokenType::OpPlus, "+"));
            },
            '-' => {
                input.remove(0);
                return Some(Token::new(TokenType::OpMinus, "-"));
            },
            '*' => {
                input.remove(0);
                return Some(Token::new(TokenType::OpStar, "*"));
            },
            '/' => {
                input.remove(0);
                return Some(Token::new(TokenType::OpDiv, "/"));
            },
            '^' => {
                input.remove(0);
                return Some(Token::new(TokenType::OpExp, "^"));
            },
            '(' => {
                input.remove(0);
                return Some(Token::new(TokenType::OpenParen, "("));
            },
            ')' => {
                input.remove(0);
                return Some(Token::new(TokenType::CloseParen, ")"));
            },
            _ => {
                // Ignore spaces
                if input.chars().next().unwrap().is_whitespace() {
                    input.remove(0);
                    return Some(Token::new_with_type(TokenType::IgnSpace));
                }

                if input.chars().next().unwrap().is_digit(10) {
                    let mut val = String::new();
                    let mut seen = false;

                    while input.chars().next().unwrap().is_digit(10) ||
                          input.chars().next().unwrap() == '.' && !input.is_empty() {
                        let c = input.chars().next().unwrap();

                        if c == '.' && seen {
                            input.remove(0);
                            continue;
                        }

                        if c == '.' {
                            seen = true;
                        }

                        val.push(c);
                        input.remove(0);
                    }

                    return Some(Token::new(TokenType::Number, &val));
                }

                return None;
            }
        }
    }
}

/// Represents a Stack
pub struct Stack {
    internal: Vec<Token>,
}

impl Stack {
    /// Constructs a new Stack
    pub fn new() -> Stack {
        Stack {
            internal: Vec::<Token>::new(),
        }
    }

    /// Pushes a token onto the Stack
    ///
    /// Parameters:
    /// * `token`: token to push 
    pub fn push(&mut self, token: Token) {
        self.internal.push(token);
    }

    /// Removes a token from the Stack
    pub fn pop(&mut self) -> Token {
        let ret = self.internal.pop().unwrap();
        ret
    }

    /// Returns true if the Stack is empty
    pub fn is_empty(&self) -> bool {
        self.internal.is_empty()
    }
}

