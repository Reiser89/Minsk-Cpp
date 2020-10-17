#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctype.h>
#include <sstream>

using std::string;
using std::list;
using std::vector;

enum class SyntaxKind
{
    NumberToken,
    WhitespaceToken,
    PlusToken,
    MinusToken,
    StarToken,
    SlashToken,
    OpenParenthesisToken,
    CloseParenthesisToken,
    BadToken,
    EndOfFileToken,
    NumberExpression,
    BinaryExpression
};

const char* SyntaxKindName[] =
{
    "NumberToken",
    "WhitespaceToken",
    "PlusToken",
    "MinusToken",
    "StarToken",
    "SlashToken",
    "OpenParenthesisToken",
    "CloseParenthesisToken",
    "BadToken",
    "EndOfFileToken",
    "NumberExpression",
    "BinaryExpression"
};

class SyntaxNode // abstract because its the base type for all syntax nodes
{
public:
    virtual SyntaxKind GetKind() = 0;

    virtual vector<SyntaxNode> GetChildren() = 0;
};

class SyntaxToken : public SyntaxNode
{
private:
    SyntaxKind Kind;
    int Position;
    string Text;
    //object Value;

public:
    SyntaxToken(SyntaxKind kind, int position, string text)
        //SyntaxToken(SyntaxKind kind, int position, string text, object value)
    {
        Kind = kind;
        Position = position;
        Text = text;
        //Value = value;
    }

    virtual SyntaxKind GetKind() { return Kind; }
    virtual int GetPosition() { return Position; }
    virtual string GetText() { return Text; }
    //virtual SyntaxKind GetValue() { return Value; }

    virtual vector<SyntaxNode> GetChildren()
    {
        return vector<SyntaxNode>();
    }
};

class Lexer
{
private:
    string _text;
    unsigned int _position;
    vector<string> _diagnostics;

public:
    Lexer(string text)
    {
        _text = text;
    }

    vector<string> GetDiagnostics()
    {
        return _diagnostics;
    }
    //public IEnumerable<string> Diagnostics = > _diagnostics;

private:
    char Current()
    {
        if (_position >= _text.length())
            return '\0';

        return _text[_position];
    }

    void Next()
    {
        _position++;
    }

public:
    SyntaxToken NextToken() // Find next word at current position of file
    {
        // <numbers>
        // + - * / ( )
        // <whitespace>

        if (_position >= _text.length())
        {
            return SyntaxToken(SyntaxKind::EndOfFileToken, _position, "\0");
            //return new SyntaxToken(SyntaxKind::EndOfFileToken, _position, "\0", null);
        }

        if (isdigit(Current()))
        {
            int start = _position;

            while (isdigit(Current()))
                Next();

            int length = _position - start;
            string text = _text.substr(start, length);

            //int.TryParse(text, out var value);
            // https://www.techiedelight.com/convert-string-to-int-cpp/
            try
            {
                int i = std::stoi(text);
            }
            catch (std::invalid_argument const&)
            {
                //_diagnostics.Add($"The number {_text} isn't valid Int32.");
                std::ostringstream tmpString;
                tmpString << "The number " << text << " isn't valid Int32.";
                _diagnostics.push_back(tmpString.str());
            }
            catch (std::out_of_range const&)
            {
                std::ostringstream tmpString;
                tmpString << "The number " << text << " isn't valid Int32.";
                _diagnostics.push_back(tmpString.str());
            }


            //return new SyntaxToken(SyntaxKind.NumberToken, start, text, value);
            return SyntaxToken(SyntaxKind::NumberToken, start, text);
        }

        if (isspace(Current()))
        {
            int start = _position;

            while (isspace(Current()))
                Next();

            int length = _position - start;
            string text = _text.substr(start, length);
            //return new SyntaxToken(SyntaxKind.WhitespaceToken, start, text, null);
            return SyntaxToken(SyntaxKind::WhitespaceToken, start, text);
        }

        if (Current() == '+')
            return SyntaxToken(SyntaxKind::PlusToken, _position++, "+");
        if (Current() == '-')
            return SyntaxToken(SyntaxKind::MinusToken, _position++, "-");
        if (Current() == '*')
            return SyntaxToken(SyntaxKind::StarToken, _position++, "*");
        if (Current() == '/')
            return SyntaxToken(SyntaxKind::SlashToken, _position++, "/");
        if (Current() == '(')
            return SyntaxToken(SyntaxKind::OpenParenthesisToken, _position++, "(");
        if (Current() == ')')
            return SyntaxToken(SyntaxKind::CloseParenthesisToken, _position++, ")");

        std::ostringstream tmpString;
        tmpString << "ERROR: bad character input: '" << Current() << "'";
        _diagnostics.push_back(tmpString.str());
        //_diagnostics.Add($"ERROR: bad character input: '{Current}'");
        //return new SyntaxToken(SyntaxKind.BadToken, _position++, _text.Substring(_position - 1, 1), null);
        return SyntaxToken(SyntaxKind::BadToken, _position++, _text.substr(_position - 1, 1));
    }
};

int main()
{
    while (true)
    {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty())
            return 0;

        Lexer lexer = Lexer(line);

        while (true)
        {
            auto token = lexer.NextToken();

            if (token.GetKind() == SyntaxKind::EndOfFileToken)
                break;

            std::cout << SyntaxKindName[static_cast<int>(token.GetKind())] << ": '" << token.GetText() << "'\n";
            //Console.Write($"{token.Kind}: '{token.Text}'");

            // if (token.Value != null)
            //     Console.Write($" {token.Value}");

            //Console.WriteLine();
        }


    }
}
