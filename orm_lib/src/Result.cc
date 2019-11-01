/**
 *
 *  Result.cc
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#include "ResultImpl.h"
#include <assert.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/ResultIterator.h>
#include <drogon/orm/Row.h>

using namespace drogon::orm;

Result::ConstIterator Result::begin() const noexcept
{
    return ConstIterator(*this, (SizeType)0);
}
Result::ConstIterator Result::cbegin() const noexcept
{
    return begin();
}
Result::ConstIterator Result::end() const noexcept
{
    return ConstIterator(*this, size());
}
Result::ConstIterator Result::cend() const noexcept
{
    return end();
}

Result::ConstReverseIterator Result::rbegin() const
{
    return ConstReverseResultIterator(end());
}
Result::ConstReverseIterator Result::crbegin() const
{
    return rbegin();
}
Result::ConstReverseIterator Result::rend() const
{
    return ConstReverseResultIterator(begin());
}
Result::ConstReverseIterator Result::crend() const
{
    return rend();
}

Result::ConstIterator Result::ConstReverseIterator::base() const noexcept
{
    iterator_type tmp(*this);
    return ++tmp;
}

Result::Reference Result::front() const noexcept
{
    return Row(*this, 0);
}

Result::Reference Result::back() const noexcept
{
    return Row(*this, size() - 1);
}

Result::Reference Result::operator[](SizeType index) const
{
    assert(index < size());
    return Row(*this, index);
}
Result::Reference Result::at(SizeType index) const
{
    return operator[](index);
}

ConstResultIterator ConstResultIterator::operator++(int)
{
    ConstResultIterator old(*this);
    _index++;
    return old;
}
ConstResultIterator ConstResultIterator::operator--(int)
{
    ConstResultIterator old(*this);
    _index--;
    return old;
}

ConstReverseResultIterator ConstReverseResultIterator::operator++(int)
{
    ConstReverseResultIterator old(*this);
    iterator_type::operator--();
    return old;
}

ConstReverseResultIterator ConstReverseResultIterator::operator--(int)
{
    ConstReverseResultIterator old(*this);
    iterator_type::operator++();
    return old;
}

Result::SizeType Result::size() const noexcept
{
    return _resultPtr->size();
}
void Result::swap(Result &other) noexcept
{
    _resultPtr.swap(other._resultPtr);
}
Result::RowSizeType Result::columns() const noexcept
{
    return _resultPtr->columns();
}
const char *Result::columnName(Result::RowSizeType number) const
{
    return _resultPtr->columnName(number);
}
Result::SizeType Result::affectedRows() const noexcept
{
    return _resultPtr->affectedRows();
}
Result::RowSizeType Result::columnNumber(const char colName[]) const
{
    return _resultPtr->columnNumber(colName);
}
const char *Result::getValue(Result::SizeType row,
                             Result::RowSizeType column) const
{
    return _resultPtr->getValue(row, column);
}
bool Result::isNull(Result::SizeType row, Result::RowSizeType column) const
{
    return _resultPtr->isNull(row, column);
}
Result::FieldSizeType Result::getLength(Result::SizeType row,
                                          Result::RowSizeType column) const
{
    return _resultPtr->getLength(row, column);
}
unsigned long long Result::insertId() const noexcept
{
    return _resultPtr->insertId();
}
const std::string &Result::sql() const noexcept
{
    return _resultPtr->sql();
}

int Result::oid(RowSizeType column) const noexcept
{
    return _resultPtr->oid(column);
}