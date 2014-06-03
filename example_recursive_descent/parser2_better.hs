import Text.Parsec ((<|>), char, parse, try)

expr =  try (expr >> symbol '+' >> term)
    <|> try (expr >> symbol '-' >> term)
    <|> term

term =  try (term >> symbol '*' >> factor)
    <|> try (term >> symbol '/' >> factor)
    <|> factor

factor = try (symbol '(' >> expr >> symbol ')')
    <|> number

number = try (digit >> number)
    <|> digit

digit = symbol '0'
    <|> symbol '1'
    <|> symbol '2'
    <|> symbol '3'
    <|> symbol '4'
    <|> symbol '5'
    <|> symbol '6'
    <|> symbol '7'
    <|> symbol '8'
    <|> symbol '9'

symbol c = char c >> return ()

main = case parse expr "" "11+2*3" of
    Left err -> print err
    Right _  -> print "ok:)"
