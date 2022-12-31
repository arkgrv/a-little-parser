use std::process::exit;
use crate::parser::{Token, TokenType, Stack};

/// Calculates an expression
///
/// Parameters:
/// * `tokens`: vector of tokens representing the expression
pub fn calculate(mut tokens: Vec<Token>) -> Option<f64> {
    let mut last_fives = Vec::<Token>::with_capacity(5);
    last_fives.resize(5, Token::new_empty());
    let mut stack = Stack::new();

    let mut acc = 0_f64;

    while !tokens.is_empty() {
        let mut tok = tokens.first().unwrap().clone();
        stack.push(tok.clone());
        tokens.remove(0);

        if tok.t_type != TokenType::CloseParen {
            continue;
        }
        
        for i in (0..5).rev() {
            last_fives[i] = stack.pop();
        }

        // Syntax check
        if last_fives[0].t_type != TokenType::OpenParen { break; }
        if last_fives[1].t_type != TokenType::Number { break; }
        if last_fives[2].t_type != TokenType::OpPlus &&
           last_fives[2].t_type != TokenType::OpMinus &&
           last_fives[2].t_type != TokenType::OpStar &&
           last_fives[2].t_type != TokenType::OpDiv &&
           last_fives[2].t_type != TokenType::OpExp { break; }
        if last_fives[4].t_type != TokenType::CloseParen { break; }

        let a = last_fives[1].value.parse::<f64>();
        let b = last_fives[3].value.parse::<f64>();

        if a.is_err() || b.is_err() {
            return None;
        }

        let a = a.unwrap();
        let b = b.unwrap();

        match last_fives[2].t_type {
            TokenType::OpPlus => acc = a + b,
            TokenType::OpMinus => acc = a - b,
            TokenType::OpStar => acc = a * b,
            TokenType::OpDiv => acc = a / b,
            TokenType::OpExp => acc = a.powf(b),
            _ => { eprintln!("Syntax error!"); exit(1); },
        }

        tok.value = acc.to_string();
        tok.t_type = TokenType::Number;

        stack.push(tok);
    }

    if !tokens.is_empty() {
        eprintln!("Syntax error!");
        exit(1);
    }

    let tok = stack.pop();
    if !stack.is_empty() || tok.t_type != TokenType::Number {
        eprintln!("Syntax error!");
        exit(1);
    }

    Some(acc)
}
