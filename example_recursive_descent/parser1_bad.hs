import Text.Parsec ((<|>), char, parse)

expr =  (expr >> symbol '+' >> term)
    <|> (expr >> symbol '-' >> term)
    <|> term

term =  (term >> symbol '*' >> factor)
    <|> (term >> symbol '/' >> factor)
    <|> factor

factor = (symbol '(' >> expr >> symbol ')')
    <|> number

number = (digit >> number)
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
