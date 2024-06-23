#pragma once

template<typename Iter>
concept ConceptIter = requires(Iter i, Iter j, int k) {
	{++i} -> std::same_as <Iter&>;
	{i++} -> std::same_as <Iter>;
	{i != j} -> std::same_as <bool>;
	{i == j} -> std::same_as <bool>;
	{*i} -> std::same_as<std::iter_reference_t<Iter>>;
	*i = *j;
	i = j;
};

template<typename Iter, typename F>
concept ConceptPred = requires(F pred1, F pred2, Iter i) {
	{pred1(*i)} -> std::same_as <bool>;
	{pred1(*i) == pred2(*i)} -> std::same_as<bool>;
	{pred1(*i) != pred2(*i)} -> std::same_as<bool>;
};

template<typename Iter, typename F>
concept ConceptFunc = requires(F lambda, Iter i) {
	lambda(*i);
};

template<typename Iterator, typename Predicate>
requires ConceptIter<Iterator>&& ConceptPred<Iterator, Predicate>
Iterator findIf(Iterator first, Iterator last, Predicate pred)
{
	while (first != last) {
		if (pred(*first)) return first;
		++first;
	}
	return last;
}




template <typename Iterator>
requires ConceptIter<Iterator>
Iterator minElement(Iterator first, Iterator last)
{
	if (first == last) return last;
	Iterator smallest = first;

	while (++first != last)
		if (*first < *smallest)
			smallest = first;
	return smallest;
}

template <typename Iterator>
requires ConceptIter<Iterator>
Iterator maxElement(Iterator first, Iterator last)
{
	if (first == last) return last;
	Iterator largest = first;

	while (++first != last)
		if (*largest < *first)
			largest = first;
	return largest;
}

template<typename Iterator, typename Function>
requires ConceptIter<Iterator>&& ConceptFunc <Iterator, Function>
void forEach(Iterator first, Iterator last, Function fn)
{
	while (first != last) {
		fn(*first);
		++first;
	}
}

template <typename Iterator, typename Predicate>
requires ConceptIter<Iterator>&& ConceptPred<Iterator, Predicate>
Iterator copyIf(Iterator first, Iterator last, Iterator result, Predicate pred)
{
	while (first != last) {
		if (pred(*first)) {
			*result = *first;
			++result;
		}
		++first;
	}
	return result;
}

template <typename Iterator, typename Function>
requires ConceptIter<Iterator>&& ConceptPred<Iterator, Function>
void Sort(Iterator beg, Iterator end, Function Fn)
{
	while (beg < --end){
		for (Iterator i = beg; i < end; ++i){
			Iterator iplus = i + 1;
			if (Fn(*iplus, *i)){
				std::iter_swap(i, iplus);
			}
		}
	}
}
