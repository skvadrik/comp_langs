import Text.Parsec ((<|>), char, parse, try)

data AST
    = Number Int
    | Add AST AST
    | Sub AST AST
    | Mul AST AST
    | Div AST AST
    deriving (Show)

expr = term >>= expr1

expr1 n1 =
        try (char '+' >> term >>= \ n2 -> expr1 (Add n1 n2))
    <|> try (char '-' >> term >>= \ n2 -> expr1 (Sub n1 n2))
    <|> return n1

term = factor >>= term1

term1 n1 =
        try (char '*' >> factor >>= \ n2 -> term1 (Mul n1 n2))
    <|> try (char '/' >> factor >>= \ n2 -> term1 (Div n1 n2))
    <|> return n1

factor = try (char '(' >> expr >>= \ n -> char ')' >> return n)
    <|> (number 0 >>= \ i -> return (Number i))

number i = try (digit >>= \ d -> number (i * 10 + d))
    <|> (digit >>= \ d -> return (i * 10 + d))

digit = (char '0' >> return 0)
    <|> (char '1' >> return 1)
    <|> (char '2' >> return 2)
    <|> (char '3' >> return 3)
    <|> (char '4' >> return 4)
    <|> (char '5' >> return 5)
    <|> (char '6' >> return 6)
    <|> (char '7' >> return 7)
    <|> (char '8' >> return 8)
    <|> (char '9' >> return 9)

main = case parse expr "" "11+2*3/(4-567)" of
    Left err  -> print err
    Right ast -> print ast
