# -*- coding : utf-8 -*-
class ParserError(Exception): # 
    pass
    
class Sentence(object): # 定义一个Sentence类，初识化属性有subject、verb和object
    def __init__(self, subject, verb, object):
        # remember we take ('noun', 'princess') tuples and convert them
        self.subject = subject[1] # 取词汇元组第二个，即实际词汇
        self.verb = verb[1]
        self.object = object[1] 
        
def peek(word_list):
    if word_list:
        word = word_list[0] # 取词汇元组并返回类型值
        return word[0]
    else:
        return None
    
def match(word_list, expecting): 
     if word_list:
        word = word_list.pop(0) # 删除相应词汇元组并返回该元组
            
        if word[0] == expecting: # 词汇类型检测
            return word
        else:
            return None
     else:
        return None
    
def skip(word_list, word_type):
     while peek(word_list) == word_type: # 词汇类型检测
        match(word_list, word_type) 
            
def parse_verb(word_list): # 处理动词
     skip(word_list, 'stop') # 停用词跳过
        
     if peek(word_list) == 'verb': # 动词则返回匹配的该词汇元组
        return match(word_list, 'verb')
     else:  # 否则抛出ParseError
        raise ParserError("Expected a verb next.") 
    
def parse_object(word_list): # 处理宾语
	 skip(word_list, 'stop') # 跳过停用词
	 next = peek(word_list) # 窥视下一个词汇元组
		
	 if next == 'noun': 如为名词，返回匹配的词汇元组
		return match(word_list, 'noun')
	 if next == 'direction': # 方位词同上
		return match(word_list, 'direction')
	 else:
		raise ParserError("Expected a noun or direction next.")    
    
def parse_subject(word_list, subj): # 处理主语
     verb = parse_verb(word_list)
     obj = parse_object(word_list)
        
     return Sentence(subj, verb, obj)
        
def parse_sentence(word_list):
     skip(word_list, 'stop')
        
     start = peek(word_list)
        
     if start == "noun":
        subj = match(word_list, 'noun')
        return parse_subject(word_list, subj)   
     elif start == 'verb':
        # assume the subject is the player then
        return parse_subject(word_list, ('noun', 'player'))
     else:
        raise ParserError("Must start with subject, object, or verb not: %s" % start)