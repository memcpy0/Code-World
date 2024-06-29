from nose.tools import *
from ex49 import ex49

def test_peek():
    assert_equal(ex49.peek([('direction', 'east')]), 'direction')
    assert_equal(ex49.peek([ ]), None)
    result = ex49.peek([('noun', 'east'),
                                   ('verb', 'go')])
    assert_equal(result, 'noun')
    
def test_match():
    assert_equal(ex49.match([('verb', 'kill')], 'verb'), ('verb', 'kill'))
    assert_equal(ex49.match([('noun', 'bear')], 'verb'), None)
    assert_equal(ex49.match([ ], 'noun'), None)

def test_skip():
    assert_equal(ex49.skip([('stop','the')], 'stop'), None)
    result = ex49.skip([('stop', 'in'), ('noun','bear')], 'stop')
    assert_equal(result, None)
    
def test_parse_verb():
    assert_raises(ex49.ParserError, ex49.parse_verb, [('stop','the'), ('stop','in')])
    assert_equal(ex49.parse_verb([('stop','the'), ('verb', 'go')]), ('verb', 'go'))
    assert_equal(ex49.parse_verb([('verb', 'eat'),('verb', 'kill')]), ('verb', 'eat'))
    
    assert_raises(ex49.ParserError, ex49.parse_verb, [('noun', 'door'), ('verb', 'go')])
    assert_raises(ex49.ParserError, ex49.parse_verb, [('direction', 'east'), ('verb', 'kill')])

def test_parse_object():
    assert_raises(ex49.ParserError, ex49.parse_object, [('stop','the'), ('stop','in')])
    assert_raises(ex49.ParserError, ex49.parse_object, [('stop','the'), ('verb', 'go')])
    
    assert_equal(ex49.parse_object([('noun', 'door'), ('verb', 'go')]), ('noun', 'door'))
    
    result = ex49.parse_object([('direction', 'north'), ('verb', 'eat')])
    assert_equal(result, ('direction', 'north'))
    
def test_parse_subject():
    assert_equal(ex49.parse_subject([('verb', 'kill'), ('noun', 'bear')], ('noun', 'doctor')), ex49.Sentence(('noun', 'doctor'), ('verb', 'kill'), ('noun', 'bear')))
    
    assert_equal(ex49.parse_subject([('stop','in'), ('verb', 'go'), ('stop','the'), ('direction', 'west')], ('noun', 'teacher')), ex49.Sentence(('noun', 'teacher'), ('verb', 'go'), ('direction', 'west')))
    
def test_parse_sentence():
    assert_raises(ex49.ParserError, ex49.parse_sentence, [('stop', 'at'), ('noun', 'door')])
    
    assert_equal(ex49.parse_sentence([('stop', 'in'), ('stop', 'at'), ('verb', 'kill'), ('noun', 'bear')]), ex49.parse_subject([('stop', 'in'), ('stop', 'at'), ('verb', 'kill'), ('noun', 'bear')], ('noun', 'player')))
    
    assert_raises(ex49.ParserError, ex49.parse_sentence, [('stop', 'in'), ('stop', 'at')])
    
    assert_equal(ex49.parse_sentence([('noun', 'door'), ('verb', 'open')]), ex49.Sentence(('noun', 'door'), ('verb', 'open'), None))
    
    assert_equal(ex49.parse_sentence([('verb', 'kill'), ('stop', 'the'), ('noun', 'bear')]), ex49.Sentence(('noun', 'player'), ('verb', 'kill'), ('noun', 'bear')))
    