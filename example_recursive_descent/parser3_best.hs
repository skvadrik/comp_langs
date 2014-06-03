import Text.Parsec ((<|>), char, parse, try)

expr = term >> expr1

expr1 = try (symbol '+' >> term >> expr1)
    <|> try (symbol '-' >> term >> expr1)
    <|> return ()

term = factor >> term1

term1 = try (symbol '*' >> factor >> term1)
    <|> try (symbol '/' >> factor >> term1)
    <|> return ()

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
