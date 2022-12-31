pub mod parser;
pub mod calculator;

use std::{io, process::exit};
use calculator::calculate;

use crate::parser::{Token, TokenType};

fn main() {
    let mut line = String::new();
    io::stdin().read_line(&mut line).expect("Cannot read from stdin!");
    let tokens = read(&line);
    let result = calculate(tokens);

    if result.is_some() {
        println!("{}", result.unwrap());
    } else {
        println!("Error during calculation!");
    }
}

fn read(line: &String) -> Vec<Token> {
    let mut res = Vec::<Token>::new();
    let mut text = line.clone();
    
    loop {
        let token = Token::next_token(&mut text);
        if token.is_none() && text.is_empty() {
            break;
        }

        if token.is_none() {
            eprintln!("Unknown token in {}", text);
            exit(1);
        }

        let token = token.unwrap();
        if token.t_type != TokenType::IgnSpace {
            res.push(token);
        }
    }

    res
}
